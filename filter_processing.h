#pragma once

#include "parser.h"
#include "filters.h"
#include "map"

namespace FilterFactories {
BaseFilter* MakeCrop(const FilterDescriptor& fd);

BaseFilter* MakeNeg(const FilterDescriptor& fd);

BaseFilter* MakeGreyScaleFilter(const FilterDescriptor& fd);

BaseFilter* MakeSharpeningFilter(const FilterDescriptor& fd);
}

class FilterPipelineFactory {
public:
    FilterPipelineFactory()
        : filter_to_makers_({{"-crop", &FilterFactories::MakeCrop},
                             {"-neg", &FilterFactories::MakeNeg},
                             {"-gs", &FilterFactories::MakeGreyScaleFilter},
                             {"-sharp", &FilterFactories::MakeSharpeningFilter}}) {
    }

public:
    using FilterFactory = BaseFilter* (*)(const FilterDescriptor&);
    using FilterToMakerMap = std::map<std::string_view, FilterFactory>;

public:
    void AddFilterMaker(std::string_view name, FilterFactory factory);

    FilterFactory GetFilterFactoryMaker(std::string_view name) const;

    BaseFilter* CreateFilter(const FilterDescriptor& descriptor) const;

protected:
    FilterToMakerMap filter_to_makers_;
};
