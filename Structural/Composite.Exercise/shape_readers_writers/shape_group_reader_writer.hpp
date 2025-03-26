#ifndef SHAPEGROUPREADERWRITER_HPP
#define SHAPEGROUPREADERWRITER_HPP

#include "../shape_factories.hpp"
#include "../shape_group.hpp"
#include "shape_reader_writer.hpp"

namespace Drawing
{
    namespace IO
    {
        class ShapeGroupReaderWriter : public ShapeReaderWriter
        {
            ShapeFactory& shape_factory_;
            ShapeRWFactory& shape_rw_factory_;
        public:
            ShapeGroupReaderWriter(ShapeFactory& shape_factory, ShapeRWFactory& shape_rw_factory)
                : shape_factory_{shape_factory}, shape_rw_factory_{shape_rw_factory}
            {
            }

            void read(Shape& shp, std::istream& in) override
            {
                ShapeGroup& shape_group = static_cast<ShapeGroup&>(shp);

                int count;
                in >> count;

                for (int i = 0; i < count; ++i)
                {
                    std::string id;
                    in >> id;

                    auto shape = shape_factory_.create(id);
                    auto shape_rw = shape_rw_factory_.create(make_type_index(*shape));
                    shape_rw->read(*shape, in);

                    shape_group.add(std::move(shape));
                }
            }

            void write(const Shape& shp, std::ostream& out) override
            {
                // const ShapeGroup& shape_group = static_cast<const ShapeGroup&>(shp);

                // out << ShapeGroup::id << " " << shape_group.size() << std::endl;

                // for (const auto& shape : shape_group)
                // {
                //     auto shape_rw = shape_rw_factory_.create(make_type_index(*shape));
                //     shape_rw->write(*shape, out);                    
                // }
            }
        };
    }
}

#endif // SHAPEGROUPREADERWRITER_HPP
