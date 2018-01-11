/*
 * KeyGen is a key- and password generator.
 * Copyright (C) 2014-2018  offa
 *
 * This file is part of KeyGen.
 *
 * KeyGen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KeyGen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KeyGen.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <CppUTest/TestHarness.h>
#include <string.h>
#include "keygen/KeyGen.h"
#include "TestUtil.h"

extern const char ALPHANUMERIC_CHARS[];
extern const size_t ALPHANUMERIC_LENGTH;
extern const char ASCII_REDUCED_CHARS[];
extern const size_t ASCII_REDUCED_LENGTH;

TEST_GROUP(FormatTest)
{
    void setup() override
    {
        size = 2000 * sizeof(uint8_t);
        buffer = allocate(size);
    }

    void teardown() override
    {
        keygen_cleanAndFreeBuffer(buffer, size);
    }

    bool isIn(const char allowedChars[], size_t allowedCharsSize, char testFor) const
    {
        for( size_t i=0; i<allowedCharsSize; ++i )
        {
            if( strchr(allowedChars, testFor) != NULL )
            {
                return true;
            }
        }

        return false;
    }

    bool testFormat(const uint8_t* inBuffer, size_t inBufferSize, enum Format format) const
    {
        for( size_t i = 0; i < inBufferSize; ++i )
        {
            switch( format )
            {
                case ASCII:
                    CHECK_FALSE(inBuffer[i] <= ' ' || inBuffer[i] > '~');
                    return true;
                case ASCII_BLANKS:
                    CHECK_FALSE(inBuffer[i] < ' ' || inBuffer[i] > '~');
                    return true;
                case ASCII_REDUCED:
                    CHECK_TRUE(isIn(ASCII_REDUCED_CHARS, ASCII_REDUCED_LENGTH, inBuffer[i]));
                    return true;
                case ALPHA_NUMERIC:
                    CHECK_TRUE(isalnum(inBuffer[i]))
                    return true;
                default:
                    break;
            }
        }

        return false;
    }

    size_t size;
    uint8_t* buffer;
};

TEST(FormatTest, formatAscii)
{
    constexpr Format format = ASCII;
    const KeyGenError rtn = keygen_createKey(buffer, size, format);
    CHECK_EQUAL(KG_ERR_SUCCESS, rtn);
    CHECK_EQUAL(true, testFormat(buffer, size, format));
}

TEST(FormatTest, formatAsciiBlanks)
{
    constexpr Format format = ASCII_BLANKS;
    const KeyGenError rtn = keygen_createKey(buffer, size, format);
    CHECK_EQUAL(KG_ERR_SUCCESS, rtn);
    CHECK_EQUAL(true, testFormat(buffer, size, format));
}

TEST(FormatTest, formatAsciiReduced)
{
    constexpr Format format = ASCII_REDUCED;
    const KeyGenError rtn = keygen_createKey(buffer, size, format);
    CHECK_EQUAL(KG_ERR_SUCCESS, rtn);
    CHECK_EQUAL(true, testFormat(buffer, size, format));
}

TEST(FormatTest, formatAlphaNumeric)
{
    constexpr Format format = ALPHA_NUMERIC;
    const KeyGenError rtn = keygen_createKey(buffer, size, format);
    CHECK_EQUAL(KG_ERR_SUCCESS, rtn);
    CHECK_EQUAL(true, testFormat(buffer, size, format));
}

TEST(FormatTest, formatIllegal)
{
    constexpr Format format = static_cast<Format>(99);
    const KeyGenError rtn = keygen_createKey(buffer, size, format);
    CHECK_EQUAL(KG_ERR_ILL_ARGUMENT, rtn);
}

