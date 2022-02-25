#include <memory>
#include <mutex>
#include <sys/types.h>
#include <math.h>
#include <netinet/ip.h>

//#include "config.hpp"
//#include "batscodenhop.hpp"


#pragma pack(push, 1)
struct BatsHeader {
  struct ip iph;
  uint64_t encoderId;
  uint16_t fileId;
  uint16_t numOfPackets;
  // uint32_t checkSum;  TODO: add checksum
  //BatchParam param;
};
#pragma pack(pop)

#define MTU (20 + 1480)  // need to be 20 + multiple of 8
#define BATCH_SIZE 4
#define COEF_HEADER_LENGTH BATCH_SIZE  // TODO: change this to BATCH_SIZE * OUTER CODE FIELD SIZE
#define ENCODED_PKT_SIZE  (MTU-sizeof(struct BatsHeader)-COEF_HEADER_LENGTH)
#define MAX_FILE_ID 30000

#define PAD32(x) (((x) + 31) & -32)
#define FLEXIBLE_PKT_SIZE true

#define IPPROTO_BATS 225
#define MAX_PACKET_SIZE 66000  // need to be greater than 65507 for netfilter headers
#define MAX_FILE_SIZE 65540
#define MAX_FILE_SEGMENTS MAX_FILE_SIZE

/**
 * @brief Batch ID type
 */
typedef uint16_t IdType;

struct BatchParam {
    /**
     * @brief Construct a new Batch Param object
     */
    BatchParam() {}
    BatchParam(IdType batch_id, int degree)
        : batch_id(batch_id), degree(degree) {}

    IdType batch_id = 0;
    int degree = 0;
};

void packetSizeOptimizer(int32_t len, double OVERHEAD ,int &batchNum, int &numOfPackets, int &opt_pkt_size)
{
  int filesize;
  int max_pkt_size = ENCODED_PKT_SIZE;
  int protocol_overhead = MTU-ENCODED_PKT_SIZE;
  
  // Round up filesize to be multiple of ENCODED_PKT_SIZE
  filesize = (len+max_pkt_size-1)/max_pkt_size*max_pkt_size;
  numOfPackets = filesize / max_pkt_size;

  // The library cannot decode if numOfPackets is between 21 to 23 (existing
  // bug) for batch size = 16 and packet size = 256
  // if (numOfPackets >= 21 && numOfPackets <= 23) {
  //   numOfPackets = 24;
  // }
  batchNum = numOfPackets * OVERHEAD / BATCH_SIZE + 1;
  opt_pkt_size = max_pkt_size;


  int stage = 0;
  // CAUTION: need to take care of precision of double
  if(FLEXIBLE_PKT_SIZE)
  {
    filesize = len;
    int min_num_pkt = ceil((double)filesize/max_pkt_size);
    numOfPackets = min_num_pkt;

    double modded_overhead = fmod(OVERHEAD, BATCH_SIZE);
	  if(modded_overhead != 0)
    {
		  int delta = ceil(min_num_pkt*modded_overhead/BATCH_SIZE);
		  double fullK1 = delta*BATCH_SIZE/modded_overhead;
		  int K1 = fullK1;
		  double fracK1 = fullK1-K1;

		  auto cal_overall_size = [=](int K)->int{
			return ceil(K*OVERHEAD/BATCH_SIZE)*BATCH_SIZE*(ceil((double)filesize/K)+protocol_overhead);
		};

		int overall_size = cal_overall_size(numOfPackets);

		if(K1 > min_num_pkt && K1 <= filesize)
    {
			int temp = cal_overall_size(K1);
			if(temp < overall_size)
      {
				numOfPackets = K1;
				overall_size = temp;
				stage = 1;
			}
		}

		double phi = BATCH_SIZE/modded_overhead-floor(BATCH_SIZE/modded_overhead);
		if(phi != 0)
    {
			// for numerical stability. when fracK1 is too small, delta and K2 will be very large
			if(fabs(fracK1) < 1e-6)
				fracK1 = 0;
			delta += ceil(1-fracK1)/phi;
			int K2 = floor(delta*BATCH_SIZE/modded_overhead);
			if(K2 <= filesize && K2 > 0)
      {
				int temp = cal_overall_size(K2);
				if(temp < overall_size)
        {
					numOfPackets = K2;
					overall_size = temp;
					stage = 2;
				}
			}
		}
	}

    batchNum = ceil(numOfPackets*OVERHEAD/BATCH_SIZE);
    opt_pkt_size = ceil((double)filesize/numOfPackets);
  }
  printf("filesize = %d, numOfPackets = %d, opt_stage = %d, opt_pkt_size = %d, true_overhead = %f\n", len, numOfPackets, stage, opt_pkt_size, (double)batchNum*BATCH_SIZE*opt_pkt_size/len- 1);
}