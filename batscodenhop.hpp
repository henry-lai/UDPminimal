/**
 * @file batscodenhop.hpp
 * @author 
 * @brief 
 * @version 1.2
 * @date 2021-08-16
 * 
 * @copyright Copyright n-hop technologies Ltd.(c)2021
 * 
 * @brief This file provides the API to bats-lib
 */

#ifndef BATSCODE_HPP
#define BATSCODE_HPP

#include <stdint.h>

/**
 * @brief Finite field Symbol type
 */
typedef unsigned char SymbolType;

/**
 * @brief Batch ID type
 */
typedef uint16_t IdType;

/**
 * @brief The structure used to save parameters of each batch
 * 
 */
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

/**
 * @brief Defines the class BatsEncoder
 */
class BatsEncoder;
/**
 * @brief Defines the class BatsRecoder
 */
class BatsRecoder;
/**
 * @brief Defines the class BatsDecoder
 */
class BatsDecoder;

/**
 * @brief returns the pointer to a new BatsEncoder object
 * 
 * @param batch_size 
 * @param packet_num 
 * @param packet_size 
 * @param inpt_packets 
 * @return BatsEncoder* 
 */
BatsEncoder* BatsEncoderNoPreInit(int batch_size,
                                  int packet_num,
                                  int packet_size,
                                  SymbolType** inpt_packets);

/**
 * @brief  returns the pointer to a new BatsEncoderPi object
 * 
 * @param batch_size 
 * @param packet_num 
 * @param packet_size 
 * @param inpt_packets 
 * @return BatsEncoder* 
 */
BatsEncoder* BatsEncoderPiInit(int batch_size,
                               int packet_num,
                               int packet_size,
                               SymbolType** inpt_packets);

/**
 * @brief Generates a batch and returns the Batch Parameters of the  generated batch
 * 
 * @param encoder 
 * @param batch 
 * @return BatchParam 
 */
BatchParam BatsEncoderGenBatch(BatsEncoder* encoder, SymbolType** batch);

/**
 * @brief Deletes the encoder object
 * 
 * @param encoder 
 */
void BatsEncoderDelete(BatsEncoder* encoder);

/**
 * @brief Create a Bats Recoder object
 * 
 * @param packet_size 
 * @return BatsRecoder* 
 */
BatsRecoder* CreateBatsRecoder(int packet_size);


/**
 * @brief 
 * 
 * @param recoder 
 * @param batch_size 
 * @param input_packet 
 * @param recoded_packet 
 */
void BatsRecoderGenPacket(BatsRecoder* recoder,
                          int batch_size,
                          SymbolType** input_packet,
                          SymbolType* recoded_packet);

void BatsRecoderDelete(BatsRecoder* recoder);

/**
 * @brief Create a Bats Decoder Bp object
 * 
 * @param batch_size 
 * @param packet_num 
 * @param packet_size 
 * @param output_packet 
 * @return BatsDecoder* 
 */
BatsDecoder* CreateBatsDecoderBp(int batch_size,
                                 int packet_num,
                                 int packet_size,
                                 SymbolType** output_packet);
/**
 * @brief Create a Bats Decoder Pi object
 * 
 * @param batch_size 
 * @param packet_num 
 * @param packet_size 
 * @param output_packet 
 * @return BatsDecoder* 
 */
BatsDecoder* CreateBatsDecoderPi(int batch_size,
                                 int packet_num,
                                 int packet_size,
                                 SymbolType** output_packet);
/**
 * @brief 
 * 
 * @param decoder 
 * @param batch_id 
 * @param degree 
 * @param packet 
 * @return true 
 * @return false 
 */
bool BatsDecoderAddPacket(BatsDecoder* decoder,
                          IdType batch_id,
                          int degree,
                          SymbolType* packet);

/**
 * @brief 
 * 
 * @param decoder 
 */
void BatsDecoderDelete(BatsDecoder* decoder);

#endif