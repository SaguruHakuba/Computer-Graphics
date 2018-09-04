#ifndef __IMAGE_BUILDER
#define __IMAGE_BUILDER

#define RES 100

class ImageBuilder
{
public:
    void setup(std::vector<Colorer*> & imageColorers)
    {
        //TODO 1 : Replace the body of this method with whatever you like.
        //         You may not change the method signature.
        
        //fill some with black
        for(size_t i=0; i<imageColorers.size(); i+=3)
            imageColorers.at(i) = new BlackColorer();

		for (size_t i = 1; i < imageColorers.size(); i+=3)
			imageColorers.at(i) = new WhiteColorer();

		for (size_t i = 1; i < imageColorers.size(); i += 3)
			imageColorers.at(i) = new TrickyColorer();
    }
    
    void color(std::vector<Colorer*> const & imageColorers, std::vector<rgb> & rgbColors)
    {
        bool sizesMatch = imageColorers.size() == rgbColors.size();
        bool hasPixels = rgbColors.size() > 0;
        assert(sizesMatch);
        assert(hasPixels);
        
        for(size_t i=0; i<rgbColors.size(); i++)
            imageColorers.at(i)->colorPixel( rgbColors.at(i) );
    }
};

#endif
