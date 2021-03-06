/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <Corrade/Containers/Array.h>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/TestSuite/Compare/FileToString.h>
#include <Corrade/Utility/Directory.h>

#include "Magnum/ImageView.h"
#include "Magnum/PixelFormat.h"
#include "Magnum/Trade/AbstractImageConverter.h"

#include "configure.h"

namespace Magnum { namespace Trade { namespace Test {

class AbstractImageConverterTest: public TestSuite::Tester {
    public:
        explicit AbstractImageConverterTest();

        void exportToFile();
};

AbstractImageConverterTest::AbstractImageConverterTest() {
    addTests({&AbstractImageConverterTest::exportToFile});
}

void AbstractImageConverterTest::exportToFile() {
    class DataExporter: public Trade::AbstractImageConverter {
        private:
            Features doFeatures() const override { return Feature::ConvertData; }

            Containers::Array<char> doExportToData(const ImageView2D& image) override {
                return Containers::Array<char>::from(char(image.size().x()), char(image.size().y()));
            };
    };

    /* Remove previous file */
    Utility::Directory::rm(Utility::Directory::join(TRADE_TEST_OUTPUT_DIR, "image.out"));

    /* doExportToFile() should call doExportToData() */
    DataExporter exporter;
    ImageView2D image(PixelFormat::RGBA, PixelType::UnsignedByte, {0xfe, 0xed}, {nullptr, 0xfe*0xed*4});
    CORRADE_VERIFY(exporter.exportToFile(image, Utility::Directory::join(TRADE_TEST_OUTPUT_DIR, "image.out")));
    CORRADE_COMPARE_AS(Utility::Directory::join(TRADE_TEST_OUTPUT_DIR, "image.out"),
        "\xFE\xED", TestSuite::Compare::FileToString);
}

}}}

CORRADE_TEST_MAIN(Magnum::Trade::Test::AbstractImageConverterTest)
