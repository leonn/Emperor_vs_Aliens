#include "element.h"

Element::Element(Field *pos)
: mPosition(pos){}

Element::Element(){}

void Element::setPosition(Field *pos){ 
	mPosition = pos;
}

void Element::init(){
	loadRectangle();
	loadImage();
}

void Element::shutdown(){
    Image::release(image);
}

bool Element::isAttackable(){
	return false;
}
