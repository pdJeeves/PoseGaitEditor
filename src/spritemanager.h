#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H
#include <string>

class Sprite;

Sprite * getSprite(uint8_t part, bool gender, uint8_t age, std::string name);

#endif // SPRITEMANAGER_H
