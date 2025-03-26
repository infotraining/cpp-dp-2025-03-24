#include "shape_group_reader_writer.hpp"

using namespace std;
using namespace Drawing;
using namespace Drawing::IO;

namespace
{
    bool is_registered = 
        SingletonShapeRWFactory::instance()
            .register_creator(
                make_type_index<ShapeGroup>(), 
                []() { return make_unique<ShapeGroupReaderWriter>(SingletonShapeFactory::instance(), SingletonShapeRWFactory::instance()); }
            );
}
