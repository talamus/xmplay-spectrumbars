#ifndef TRIGRADIENT_H
#define TRIGRADIENT_H

//
//  Three colour vertical gradient
//

#include "Graphic.h"
namespace Gfx {

class TriGradient : public Fill
{
  protected:
    Colour low_;               // If y under minimum use this
    Colour med_;               // Somewhere on the middle use this
    Colour high_;              // If y over maximum use this

    int min_;                  // Minimum y value
    int max_;                  // Maximum y value

    static const int table_max = 256;
    Colour table_[table_max];  // Look-up table for the gradient

  public:
    TriGradient(const Colour & low, const Colour & med, const Colour & high);
                               // Create (and pre-calculate) a new gradient

    void setScale(const int min, const int max);
                               // Set gradient scale: [min..max] -> [0..511]

    const Colour getColour(const int x, const int y) const;
                               // Give gradient colour at screen position (x,y)
};

} // namespace Gfx
#endif // TRIGRADIENT_H
