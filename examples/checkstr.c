/* Just an simple usage example of Bjoern Hoehrmann's
 * deterministic finite Automaton UTF-8 Decoder.
 *
 * This test program: Copyright (c) 2021 - Florian Cauvin
 *------------------------------------------------------------------------------
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
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#define UTF8_DECODE_IMPLEMENTATION
#include "../utf8_decode.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define CODEPOINT_MAX           0x10FFFF
#define CODEPOINT_INVALID     0xFFFFFFFE
#define CODEPOINT_INTERRUPTED 0xFFFFFFFF

void print(uint32_t codepoint, const char* u8, uint8_t len)
{
   // print codepoint
   bool valid;
   if (codepoint > CODEPOINT_MAX) {
      valid = false;
      printf(" \033[1;31minvalid\033[m");
   } else {
      valid = true;
      printf(" \\U%05X", codepoint);
   }

   // print octal sequence
   printf("  | ");
   for (uint8_t i = 0; i < len; i++)
       printf("\\%03o", u8[i]);
   if (codepoint == CODEPOINT_INTERRUPTED) {
      valid = false;
      printf(" \033[31mEND\033[m");
      len++;
   }
   for (uint8_t i = len; i < 4; i++)
       printf("    ");
   printf(" | ");

   // print character sequence
   if (valid)
      for (uint8_t i = 0; i < len; i++)
          if ((u8[i] < 32) || (u8[i] == 127)) // control chars
             printf("\033[1;33;44m%02X\033[m", u8[i]);
          else
             printf("%c", u8[i]);
   else
      printf("\033[1;37;41m!!\033[m");
   printf("\n");
}



int main(int argc, char* argv[])
{
    if (argc != 2) {
       fprintf(stderr, "Usage: %s string\n", argv[0]);
       return EXIT_FAILURE;
    }

    printf("Codepoint | Octal sequence   | Character \n");
    printf("----------|------------------|-----------\n");

    uint32_t codepoint;  // current unicode character's codepoint
    char     chr[4];     // utf8 byte sequence of unicode character
    uint8_t  len = 0;    // and it's length (max 4)

    uint32_t state=UTF8_ACCEPT;
    for (char* c=argv[1]; *c; c++) {
        chr[len++] = *c;
        switch (utf8_decode(&state, &codepoint, *c)) {

           case UTF8_ACCEPT:
                print(codepoint, chr, len);
                len=0;
                break;

           case UTF8_REJECT:
                print(CODEPOINT_INVALID, chr, len);
                len=0;
                state=UTF8_ACCEPT;
                break;
         }
    }
    if (state != UTF8_ACCEPT) {
       print(CODEPOINT_INTERRUPTED, chr, len);
       return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
