/* isutf8 [FILE]
 * returns 0  if FILE is valid UTF8
 *         1  if FILE is not valid UTF8
 *         *  if a system error occurs
 * if no FILE is given, read from standard input intead.
 *
 * Copyright (c) 2021 - Florian Cauvin
 *
 *------------------------------------------------------------------------------
 *
 * This is a simple usage example of Bjoern Hoehrmann's
 * deterministic finite Automaton UTF-8 Decoder.
 *
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
#define  RET_SUCCESS   0 // could be stdlib.h's EXIT_SUCCESS
#define  RET_FAILURE   1 // could be stdlib,h's EXIT_FAILURE
#define  RET_ERROR     2 // return code

int main(int argc, char* argv[])
{
    FILE* file;
    switch(argc) {
       case 1:  file = stdin;
                break;
       case 2:  file = fopen(argv[1], "r");
                if (file == NULL) {
                   perror(argv[1]);
                   return RET_ERROR;
                }
                break;
       default: fprintf(stderr, "Usage: %s [file]\n", argv[0]);
                return RET_ERROR;
    }

    int      c;
    uint32_t codepoint;
    uint32_t state = UTF8_ACCEPT;
    int      ret = RET_SUCCESS;
    while((c = getc(file)) != EOF)
       if (utf8_decode(&state, &codepoint, c) == UTF8_REJECT)
          goto fail;

    if (state != UTF8_ACCEPT)
       goto fail;
    goto clean;

fail:
    ret = RET_FAILURE;
clean:
    fclose(file);
    return ret;
}
