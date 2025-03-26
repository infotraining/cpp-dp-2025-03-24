#include "bitmap.hpp"
#include <algorithm>
#include <array>
#include <deque>

using namespace std;

struct Bitmap::Impl
{
    std::deque<char> image_;

    Impl(size_t size, char fill_char) : image_(size, fill_char)
    {
    }    
};

Bitmap::Bitmap(size_t size, char fill_char) : pimpl_{make_unique<Impl>(size, fill_char)}
{
}

Bitmap::~Bitmap()
{    
}

void Bitmap::draw()
{
    cout << "Image: ";
    for (size_t i = 0; i < pimpl_->image_.size(); ++i)
        cout << pimpl_->image_[i];
    cout << endl;
}
