#include "TriGradient.h"
#include "Transition.h"
namespace Gfx {

TriGradient::TriGradient(const Colour & low, const Colour & med, const Colour & high) : low_(low), med_(med), high_(high)
{
    // Pre-calculate colour table
    for (int i = table_max; i--;) {
        real r = 2.0 * Transition::bottom(real(i) / real(table_max));
        if (r >= 1.0) {
            table_[i] = med_;
            table_[i].merge(high_, byte(0xff * (r - 1.0)));
        } else {
            table_[i] = low_;
            table_[i].merge(med_, byte(0xff * r));
        }
    }
}

void TriGradient::setScale(const int min, const int max)
{
    // Make sure minimum is minimum and maximum is maximum
    if (min_ > max_) { int swap = min_; min_ = max_; max_ = swap; }

    min_ = min;
    max_ = max;
}

const Colour TriGradient::getColour(const int x, const int y) const
{
    if (y <= min_) {
        return low_;
    } else if (y >= max_) {
        return high_;
    }
    int i = (table_max - 1)*(y - min_)/(max_ - min_);
    ASSERT(i >= 0);
    ASSERT(i < table_max);
    return table_[i];
}

} // namespace Gfx
