#include "parser.h"
#include "filter_processing.h"
#include "BMP.h"

#include <cassert>
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {

    CmdLineParser parser;
    if (!parser.Parse(argc, argv)) {
        std::cout << "Something went wrong, try again with other values";
    }
    std::cout << "successfully parsed";

    BMP bitmap;

    std::string input_filename = {parser.GetInputFileName().begin(), parser.GetInputFileName().end()};
    assert(bitmap.Load(input_filename.c_str()));
    assert(!bitmap.IsEmpty());

    FilterPipelineFactory fpf;
    for (auto descriptor : parser.GetFilterDescriptors()) {
        std::shared_ptr<BaseFilter> filter(fpf.CreateFilter(*descriptor));
        assert(filter);
        filter->Apply(bitmap);
        delete descriptor;
    }

    std::string output_filename = {parser.GetOutputFileName().begin(), parser.GetOutputFileName().end()};
    assert(bitmap.WriteF(output_filename.c_str()));
}
