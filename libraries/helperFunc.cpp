#include <chrono>
#include <math.h>
#include <cmath>
#include <thread>
#include "helperFunc.h"




float Distance(float x1,float y1,float x2,float y2) {
  float distance = pow(pow(x1-x2,2) + pow(y1-y2,2),0.5);
  return distance; 
}


void keyPressTimeFreeze() {
  std::chrono::milliseconds timespan(100); 
  std::this_thread::sleep_for(timespan);
}

sf::Vertex* DrawLine(sf::Vector2f start, sf::Vector2f end, float thickness)
{   
  sf::Vector2f dv = end - start;
  float dl = std::sqrt(dv.x * dv.x + dv.y * dv.y);
  sf::Vector2f uv = dv / dl;
  sf::Vector2f up = sf::Vector2f(-uv.x,uv.y);
  sf::Vector2f offset = up * (thickness / 2.f);

  sf::Vertex* vertexArray = new sf::Vertex[4];
  vertexArray[0] = sf::Vertex(start+offset,sf::Color(0,240,240,255));
  vertexArray[1] = sf::Vertex(end+offset,sf::Color(0,240,240,255));
  vertexArray[2] = sf::Vertex(end-offset,sf::Color(0,240,240,255));
  vertexArray[3] = sf::Vertex(start-offset,sf::Color(0,240,240,255));
  return vertexArray;

            
}
