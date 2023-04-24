#ifndef SAVELOAD_H
#define SAVELOAD_H
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string.h>
#include <fstream>
#include <iostream>
#include "blob.h"


void saveFile(int nBlobs,int nSticks,bool timeFreeze,bool mCheck,bool container,float currTime,
std::vector<Blob>& blobs,std::vector<Stick>& sticks);
std::string updateStr(std::string currStr,std::size_t LSM);
std::tuple<int,int,bool,bool,bool,float,std::vector<Blob>,std::vector<sf::CircleShape>,std::vector<Stick>> fileLoad();

#endif