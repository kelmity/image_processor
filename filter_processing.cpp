#include "filter_processing.h"

#include <stdexcept>

namespace FilterFactories {
BaseFilter* MakeCrop(const FilterDescriptor& fd) {
    if (fd.filter_name != "-crop") {
        throw std::invalid_argument("wrong crop filter descriptor");
    }
    if (fd.filter_params.size() != 2) {
        throw std::invalid_argument("wrong crop filter params size");
    }
    std::string height_str = {fd.filter_params[0].begin(), fd.filter_params[0].end()};
    std::string width_str = {fd.filter_params[1].begin(), fd.filter_params[1].end()};
    return new Crop(std::stoi(height_str), std::stoi(width_str));
}

BaseFilter* MakeNeg(const FilterDescriptor& fd) {
    if (fd.filter_name != "-neg") {
        throw std::invalid_argument("wrong negative filter descriptor");
    }
    if (!fd.filter_params.empty()) {
        throw std::invalid_argument("wrong negative filter params size");
    }
    return new Neg;
}

BaseFilter* MakeGreyScaleFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-gs") {
        throw std::invalid_argument("wrong greyscale filter descriptor");
    }
    if (!fd.filter_params.empty()) {
        throw std::invalid_argument("wrong greyscale filter params size");
    }
    return new GreyScaleFilter;
}

BaseFilter* MakeSharpeningFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-sharp") {
        throw std::invalid_argument("wrong sharpening filter descriptor");
    }
    if (!fd.filter_params.empty()) {
        throw std::invalid_argument("wrong sharpening filter params size");
    }
    return new SharpeningFilter;
}
}

void FilterPipelineFactory::AddFilterMaker(std::string_view name, FilterFactory factory) {
    filter_to_makers_.insert({name, factory});
}

FilterPipelineFactory::FilterFactory FilterPipelineFactory::GetFilterFactoryMaker(std::string_view name) const {
    FilterToMakerMap::const_iterator it = filter_to_makers_.find(name);
    if (it != filter_to_makers_.end()) {
        return it->second;
    }
    return nullptr;
}

BaseFilter* FilterPipelineFactory::CreateFilter(const FilterDescriptor& descriptor) const {
    FilterFactory gear = GetFilterFactoryMaker(descriptor.filter_name);
    if (!gear) {
        return nullptr;
    }
    BaseFilter* current_filter = gear(descriptor);
    return current_filter;
}
