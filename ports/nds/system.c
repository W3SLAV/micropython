/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <nds.h>
#include <stdio.h>
#include <string.h>

extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;

void Reset_Handler(void) __attribute__((naked));
void bare_main(void);

static void nds_init(void);

// ARM9 vector table (simplified for libnds)
const uint32_t isr_vector[] __attribute__((section(".isr_vector"))) = {
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
};

// The CPU runs this function after a reset.
void Reset_Handler(void) {
    // Set stack pointer.
    __asm volatile ("ldr sp, =_estack");

    // Copy .data section from flash to RAM.
    memcpy(&_sdata, &_sidata, (char *)&_edata - (char *)&_sdata);

    // Zero out .bss section.
    memset(&_sbss, 0, (char *)&_ebss - (char *)&_sbss);

    // Initialize the CPU and peripherals.
    nds_init();

    // Now that there is a basic system up and running, call the main application code.
    bare_main();

    // This function must not return.
    for (;;) {
    }
}

// Set up the Nintendo DS (ARM9).
static void nds_init(void) {
    // Initialize the console for printing to the screen.
    consoleDemoInit();

    // Initialize the hardware and other peripherals.
    powerOn(POWER_ALL);
    irqInit();
    irqEnable(IRQ_VBLANK);
}

// Write a character out to the console.
static inline void console_write_char(int c) {
    // Print the character to the console.
    iprintf("%c", c);
}

// Send string of given length to stdout, converting \n to \r\n.
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    while (len--) {
        if (*str == '\n') {
            console_write_char('\r');
        }
        console_write_char(*str++);
    }
}
