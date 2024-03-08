#ifndef _SOFT_SPI_H_
#define _SOFT_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Generic function definitions */

/**
 * @brief Chip Select Pin control function
 * 
 * @param[in] level Digital data level. One for high, zero for low.
*/
typedef void (*soft_spi_cs_fn)(unsigned char level);

/**
 * @brief MOSI Pin Control Function
 * 
 * @param[in] level Digital data. One for high, zero for low.
*/
typedef void (*soft_spi_mosi_fn)(unsigned char level);

/**
 * @brief MISO Pin Control Function
 * 
 * @param[in] level Digital data. One for high, zero for low.
*/
typedef unsigned char (*soft_spi_miso_fn)();

/**
 * @brief CLK Pin Control Function
 * 
 * @param[in] level Digital data. One for high, zero for low.
*/
typedef void (*soft_spi_clk_fn)(unsigned char level);

/**
 * @brief Delay function.
 * 
 * @param[in] us Delay time in microseconds.
*/
typedef void (*soft_spi_delay_fn)(unsigned us);

/**
 * @brief Generic waiting fn?
*/
typedef void (*soft_spi_waiting_fn)();

/* SPI Device Mode definition */
enum soft_spi_device_mode_t {
    SOFT_SPI_MASTER = 0, 
    SOFT_SPI_SLAVE
};

/* SPI Transmission Mode definition */
enum soft_spi_transmission_mode_t {
    SOFT_SPI_MODE_0 = 0, 
    SOFT_SPI_MODE_1, 
    SOFT_SPI_MODE_2, 
    SOFT_SPI_MODE_3
};

/* SPI bit order definition */
enum soft_spi_bit_order_t {
    SOFT_SPI_MSB_FIRST = 0, 
    SOFT_SPI_LSB_FIRST
};

/**
 * @brief Soft SPI instance
*/
struct soft_spi {
    soft_spi_cs_fn cs_fn;
    soft_spi_miso_fn miso_fn;
    soft_spi_mosi_fn mosi_fn;
    soft_spi_clk_fn clk_fn;
    soft_spi_delay_fn delay_us;
    enum soft_spi_device_mode_t device_mode;
    enum soft_spi_transmission_mode_t transmission_mode;
    enum soft_spi_bit_order_t bit_order;
    unsigned period_us;
};

/* Function definitions */

/**
 * @brief Initialize A Soft SPI instance
 * 
 * @param[out] spi Pointer to an empty soft spi instance
 * @param[in] chip_select_pin_fn Chip Select pin control function
 * @param[in] miso_pin_fn MISO pin control function
 * @param[in] mosi_pin_fn MOSI pin control function
 * @param[in] clk_fn Clock pin control function
 * @param[in] delay_fn Delay control function
 * @param[in] half_period_us half period delay time in microseconds
 * @param[in] device_mode Device mode as Master/Slave
 * @param[in] transmission_mode Data transmission mode
 * 
*/
void soft_spi_create(struct soft_spi *spi, soft_spi_cs_fn chip_select_pin_fn, soft_spi_miso_fn miso_pin_fn, 
                    soft_spi_mosi_fn mosi_pin_fn, soft_spi_clk_fn clk_fn, soft_spi_delay_fn delay_fn, 
                    unsigned half_period_us, enum soft_spi_device_mode_t device_mode, enum soft_spi_transmission_mode_t transmission_mode);

/**
 * @brief Do SPI data transfer as master device
 * 
 * @param[in] spi Soft SPI instance.
 * @param[out] buffer_in Incoming data from the SPI device.
 * @param[in] buffer_out Outgoing data to the SPI device.
 * @param[in] buffer_size Size of the data buffer in bytes.
 * 
 * @note buffer_in and buffer_out can be the same data buffer.
*/
void soft_spi_transfer(struct soft_spi *spi, void *buffer_in, void *buffer_out, unsigned buffer_size);

#ifdef SOFT_SPI_ADVANCED
unsigned char soft_spi_transfer_byte(struct soft_spi *spi, unsigned char data);
#endif

#ifdef __cplusplus
}
#endif

#endif