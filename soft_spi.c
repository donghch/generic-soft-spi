#include "soft_spi.h"

void soft_spi_create(struct soft_spi *spi, soft_spi_cs_fn chip_select_pin_fn, soft_spi_miso_fn miso_pin_fn, 
                    soft_spi_mosi_fn mosi_pin_fn, soft_spi_clk_fn clk_fn, soft_spi_delay_fn delay_fn, 
                    unsigned half_period_us, enum soft_spi_mode_t spi_mode){

    spi->cs_fn = chip_select_pin_fn;
    spi->miso_fn = miso_pin_fn;
    spi->mosi_fn = mosi_pin_fn;
    spi->clk_fn = clk_fn;
    spi->delay_us = delay_fn;
    spi->mode = spi_mode;
    spi->period_us = half_period_us;

    spi->cs_fn(1);
    
    switch (spi->mode) {
        case SOFT_SPI_MODE_0:
        case SOFT_SPI_MODE_1:
            spi->clk_fn(0);
            break;
        case SOFT_SPI_MODE_2:
        case SOFT_SPI_MODE_3:
            spi->clk_fn(1);
            break;
    }

}


/**
 * @brief Transfer a byte to the spi device
 * 
 * @param[in] spi Soft SPI instance
 * @param[in] data Data to transfer
 * 
 * @return Data from the spi device
*/
#ifndef SOFT_SPI_ADVANCED
static 
#endif
unsigned char soft_spi_transfer_byte(struct soft_spi *spi, unsigned char data) {

    unsigned char result = 0;
    
    switch(spi->mode) {
        case SOFT_SPI_MODE_0:
            for (unsigned char i = 0; i < 8; i++) {
                if (spi->bit_order == SOFT_SPI_MSB_FIRST) {
                    spi->mosi_fn((data >> (7 - i)) & 0x1);
                } else {
                    spi->mosi_fn((data >> i) & 0x1);
                }
                spi->delay_us(spi->period_us);
                spi->clk_fn(1);
                result = result | (spi->miso_fn() << i);
                spi->delay_us(spi->period_us);
                spi->clk_fn(0);
            }
            break;
        case SOFT_SPI_MODE_1:
            for (unsigned char i = 0; i < 8; i++) {
                spi->clk_fn(1);
                if (spi->bit_order == SOFT_SPI_MSB_FIRST) {
                    spi->mosi_fn((data >> (7 - i)) & 0x1);
                } else {
                    spi->mosi_fn((data >> i) & 0x1);
                }
                spi->delay_us(spi->period_us);
                spi->clk_fn(0);
                result = result | (spi->miso_fn() << i);
                spi->delay_us(spi->period_us);
            }
            break;
        case SOFT_SPI_MODE_2:
            for (unsigned char i = 0; i < 8; i++) {
                if (spi->bit_order == SOFT_SPI_MSB_FIRST) {
                    spi->mosi_fn((data >> (7 - i)) & 0x1);
                } else {
                    spi->mosi_fn((data >> i) & 0x1);
                }
                spi->delay_us(spi->period_us);
                spi->clk_fn(0);
                result = result | (spi->miso_fn() << i);
                spi->delay_us(spi->period_us);
                spi->clk_fn(1);
            }
            break;
        case SOFT_SPI_MODE_3:
            for (unsigned char i = 0; i < 8; i++) {
                spi->clk_fn(0);
                if (spi->bit_order == SOFT_SPI_MSB_FIRST) {
                    spi->mosi_fn((data >> (7 - i)) & 0x1);
                } else {
                    spi->mosi_fn((data >> i) & 0x1);
                }
                spi->delay_us(spi->period_us);
                spi->clk_fn(1);
                result = result | (spi->miso_fn() << i);
                spi->delay_us(spi->period_us);
            }
            break;
        default:
            return 0;
    }

    return result;

}

void soft_spi_transfer(struct soft_spi *spi, void *buffer_in, void *buffer_out, unsigned buffer_size) {

    spi->cs_fn(0);
    spi->delay_us(spi->period_us);
    /* Here we actually should add some delays */
    for (unsigned buf_index = 0; buf_index < buffer_size; buf_index++){
        ((unsigned char *)buffer_in)[buf_index] = soft_spi_transfer_byte(spi, ((unsigned char *)buffer_out)[buf_index]);
    }
    spi->delay_us(spi->period_us);
    spi->cs_fn(1);

}