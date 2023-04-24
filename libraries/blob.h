#ifndef BLOB_H
#define BLOB_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

class Blob {
 public: // you need to add this to declare which elements can be accessed outside the class
  int ident = 0;
  bool isAnchor{false};
  bool isMagnetized{false};
  //std::vector<int> stickedIdents;
  //
  sf::Vector2f pos;
  sf::Vector2f oldPos;

  sf::Vector2f vel;
  sf::Vector2f acc;
  float m = 1;
  float radius{10};
  sf::Color color;
  sf::Vector2f TEST;
};

class Stick {
 public:
  std::vector<int> blobIdents;
  float length;

};

#endif