/**
 StreamString.h

 Copyright (c) 2015 Markus Sattler. All rights reserved.
 This file is part of the esp8266 core for Arduino environment.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef STREAMSTRING_H_
#define STREAMSTRING_H_

// free buffer if the lasts byte has been read
#ifndef STREAMSTRING_INVALIDATE_IF_EMPTY
#define STREAMSTRING_INVALIDATE_IF_EMPTY            1
#endif

// free buffer if bigger than the assigned string instead of using realloc
#ifndef STREAMSTRING_INVALIDATE_ON_ASSIGN
#define STREAMSTRING_INVALIDATE_ON_ASSIGN           1
#endif

class StreamString: public Stream, public String {
public:

#if STREAMSTRING_INVALIDATE_ON_ASSIGN
    StreamString &operator =(const StreamString &str) {
        if (capacity > str.length()) {
            invalidate();
        }
        copy(str.c_str(), str.length());
        return *this;
    }
    StreamString &operator =(const String &str) {
        if (capacity > str.length()) {
            invalidate();
        }
        copy(str.c_str(), str.length());
        return *this;
    }
    StreamString &operator =(const char *cstr) {
        if (!cstr) {
            invalidate();
        }
        else {
            auto clen = strlen(cstr);
            if (capacity > clen) {
                invalidate();
            }
            copy(cstr, clen);
        }
        return *this;
    }
	String & operator = (const __FlashStringHelper *str) {
        if (!str) {
            invalidate();
        } else {
            auto plen = strlen_P((PGM_P)str);
            if (capacity > plen) {
                invalidate();
            }
            copy(str, plen);
        }
        return *this;
     }
#endif

    size_t write(const uint8_t *buffer, size_t size) override;
    size_t write(uint8_t data) override;

    void remove(unsigned int index, unsigned int count);

    int available() override;
    int read() override;
    int peek() override;
    void flush() override;

    inline void clear() {
        invalidate();
    }
};


#endif /* STREAMSTRING_H_ */
