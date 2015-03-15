#ifndef pic_h
#define pic_h

#include "sys.h"

/* IRQ numbers for PICs 1 & 2 */
#define PIC1_SRC_TIMER                      0
#define PIC1_SRC_KEYBOARD                   1
#define PIC1_SRC_SLAVE                      2
#define PIC1_SRC_SERIAL_2                   3
#define PIC1_SRC_SERIAL_1                   4
#define PIC1_SRC_PARALLEL_2                 5
#define PIC1_SRC_FLOPPY                     6
#define PIC1_SRC_PARALLEL1                  7
#define PIC2_SRC_RTIME_CLOCK                0
#define PIC2_SRC_CGA_VERTICAL_RETRACE       1
#define PIC2_SRC_AUX                        4
#define PIC2_SRC_FPU                        5
#define PIC2_SRC_HD                         7

/* Values for control word 1 */
#define PIC_CONTROL_INIT_INIT               0x10 /* 0b00010000 - Initialize PIC */
#define PIC_CONTROL_INIT_MODE_LTM           0x08 /* 0b00001000 - Operation mode - Level Triggered Mode */
#define PIC_CONTROL_INIT_MODE_ETM           0x00 /* 0b00000000 - Operation mode - Edge Triggered Mode */
#define PIC_CONTROL_INIT_INTERVAL_4         0x04 /* 0b00000100 - Call address interval is 4 */
#define PIC_CONTROL_INIT_INTERVAL_8         0x00 /* 0b00000000 - Call address interval is 8 */
#define PIC_CONTROL_INIT_MODE_CASCADING     0x02 /* 0b00000010 - Cascading mode */
#define PIC_CONTROL_INIT_MODE_SINGLE        0x00 /* 0b00000000 - Single mode */
#define PIC_CONTROL_INIT_EXPECT_IC4         0x01 /* 0b00000001 - Expect IC4 at init */
#define PIC_CONTROL_INIT_NO_IC4             0x00 /* 0b00000000 - Do not expect IC4 at init */

/* Values for control word 4 */
#define PIC_CONTROL_FIN_MODE_8086           0x1
#define PIC_CONTROL_FIN_MODE_MCS80          0x0
#define PIC_CONTROL_FIN_AUTO_EOI_ON         0x2
#define PIC_CONTROL_FIN_AUTO_EOI_OFF        0x0
#define PIC_CONTROL_FIN_BUFF_SELECT_MASTER  0x1
#define PIC_CONTROL_FIN_BUFF_SELECT_SLAVE   0x0
#define PIC_CONTROL_FIN_BUFFFERED_ON        0x8
#define PIC_CONTROL_FIN_BUFFFERED_OFF       0x0
#define PIC_CONTROL_FIN_FULLYNESTED_ON      0x10
#define PIC_CONTROL_FIN_FULLYNESTED_OFF     0x0

/* Command & Data register numbers */
#define PIC1_REGISTER_COMMAND               0x20 /* Write */
#define PIC1_REGISTER_STATUS                0x20 /* Read */
#define PIC1_REGISTER_DATA                  0x21 /* Write */
#define PIC1_REGISTER_IMR                   0x21 /* Read */

#define PIC2_REGISTER_COMMAND               0xA0 /* Write */
#define PIC2_REGISTER_STATUS                0xA0 /* Read */
#define PIC2_REGISTER_DATA                  0xA1 /* Write */
#define PIC2_REGISTER_IMR                   0xA1 /* Read */

/* Initialization word 1 */
#define PIC_INIT_IW1                        PIC_CONTROL_INIT_INIT | PIC_CONTROL_INIT_NO_IC4

/* Initialization word 2 */
#define PIC_INIT_IW2_PIC1                   32 /* Start right after 32 software interrupts */
#define PIC_INIT_IW2_PIC2                   40

/* Initialization word 3 */
#define PIC_INIT_IW3_PIC1                   4 /* Connection to secondary PIC */
#define PIC_INIT_IW3_PIC2                   2 /* Connection to primary PIC */

/* Initialization word 4 */
#define PIC_INIT_IW4                        PIC_CONTROL_FIN_MODE_8086

/* Control words - not used now*/
#define PIC_CONTROL_EOI                     0x20 /* 0b00100000 - End of Interrupt (ack) */

#define PIC_SEND_PIC1_COMMAND(DATA)         outportb(PIC1_REGISTER_COMMAND, DATA);
#define PIC_SEND_PIC2_COMMAND(DATA)         outportb(PIC2_REGISTER_COMMAND, DATA);

#define PIC_SEND_PIC1_DATA(DATA)            outportb(PIC1_REGISTER_DATA, DATA);
#define PIC_SEND_PIC2_DATA(DATA)            outportb(PIC2_REGISTER_DATA, DATA);

#define PIC_INIT                            PIC_SEND_PIC1_COMMAND(PIC_INIT_IW1) \
                                            PIC_SEND_PIC2_COMMAND(PIC_INIT_IW1) \
                                            PIC_SEND_PIC1_DATA(PIC_INIT_IW2_PIC1) \
                                            PIC_SEND_PIC2_DATA(PIC_INIT_IW2_PIC2) \
                                            PIC_SEND_PIC1_DATA(PIC_INIT_IW3_PIC1) \
                                            PIC_SEND_PIC2_DATA(PIC_INIT_IW3_PIC2) \
                                            PIC_SEND_PIC1_DATA(PIC_INIT_IW4) \
                                            PIC_SEND_PIC2_DATA(PIC_INIT_IW4)

void pic_init();

#endif
