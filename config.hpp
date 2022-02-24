#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <netinet/ip.h>
//#include <memory>
#include <mutex>
//#include <sys/types.h>
//#include <unordered_map>


#include "batscodenhop.hpp"


/* header structure (bytes)

IP header           20 bytes
encoder ID           8 bytes
file ID              2 bytes
pktNum               2 bytes
BatchParam           8 bytes
coeff. header*      16 bytes
----------------------------
total               56 bytes

WARNING: we assumed little endian except the IP header
TODO: we should change them to network byte order
NOTE: the BatchParam struct from the library is not tight
*/

#pragma pack(push, 1)
struct BatsHeader {
  struct ip iph;
  uint64_t encoderId;
  uint16_t fileId;
  uint16_t numOfPackets;
  // uint32_t checkSum;  TODO: add checksum
  BatchParam param;
};
#pragma pack(pop)


// -----------------------------------------------

#define ENCODE_THREADS 1
#define RECODE_THREADS 1
#define DECODE_THREADS 1

#define TTL_TEST_ENABLED true
#define SMALL_CODER_ENABLED true
#define STREAM_ENABLED true
// #define STREAM_INTERVAL_ENABLED
// const std::chrono::microseconds STREAM_INTERVAL(50);

// BATS payload size; Should be a power of 2.
// Ensure that ENCODED_PKT_SIZE + headers should not be larger than the MTU
#define MTU (20 + 1480)  // need to be 20 + multiple of 8
#define BATCH_SIZE 4
#define COEF_HEADER_LENGTH BATCH_SIZE  // TODO: change this to BATCH_SIZE * OUTER CODE FIELD SIZE
#define ENCODED_PKT_SIZE  (MTU-sizeof(struct BatsHeader)-COEF_HEADER_LENGTH)

#define FLEXIBLE_PKT_SIZE true

#define IPPROTO_BATS 225
#define MAX_PACKET_SIZE 66000  // need to be greater than 65507 for netfilter headers
#define MAX_FILE_SIZE 65536
//#define MAX_FILE_SEGMENTS (MAX_FILE_SIZE / ENCODED_PKT_SIZE)
//NOTE: Dirty quick fix
#define MAX_FILE_SEGMENTS MAX_FILE_SIZE

const std::chrono::milliseconds STREAM_TIMEOUT(10000);  // Timeout for a stream
const std::chrono::milliseconds CHECK_STREAM_TIMEOUT_INTERVAL(500);
const std::chrono::milliseconds RECODER_BATCH_TIMEOUT(20);
const std::chrono::milliseconds CHECK_RECODER_BATCH_TIMEOUT_INTERVAL(5);
const std::chrono::milliseconds DECODER_FILE_TIMEOUT(150);  // Timeout for a file
const std::chrono::milliseconds CHECK_DECODER_FILE_TIMEOUT_INTERVAL(50);
const std::chrono::milliseconds DECODER_BUFFER_TIMEOUT(100);  // Timeout to send a misordered packet
const std::chrono::milliseconds CHECK_DECODER_BUFFER_TIMEOUT_INTERVAL(25);

// the encoder will work when either a timeout expires or the bytes in the queue
// excesses ENCODE_THRESHOLD
#define ENCODE_THRESHOLD 30000  // in bytes
const std::chrono::milliseconds ENCODE_TIMEOUT(15);

#define MAX_FILE_ID 30000
#define DUMMY_FILE_ID 2000000002

#define PAD32(x) (((x) + 31) & -32)

#endif
