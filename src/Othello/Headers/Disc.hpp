//
//  Disc.hpp
//  Othello
//
//  Created by Michael Felix on 11/13/23.
//

#ifndef Disc_hpp
#define Disc_hpp

#include "GraphicObject.h"
#include "AnimatedObject.h"

namespace othello {
    class Disc: public GraphicObject, public AnimatedObject {
        private:
            static float** _circlePoints;
            static const int _numCirPoints;
            RGBColor color_; // false = white, true = black
            float size_; // how large should the disc be?
        
        public:
            Disc(TilePoint& loc, RGBColor color);
            
            void draw() const;
            void update(float dt);
        
            
            inline int getRow() {
                return (int)getX();
            }
            inline int getCol() {
                return (int)getY();
            }
            inline TilePoint getPos() {
                return TilePoint{getRow(), getCol()};
            }
            inline void setColor(RGBColor color) {
                color_ = color;
            }
            inline void setColor(float red, float green, float blue) {
                color_ = RGBColor{red, green, blue};
            }
    };
}

#endif /* Disc_hpp */
