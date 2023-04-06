#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <cmath>
#include <string>
#include <SFML/System/Vector2.hpp>
#include <random>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <chrono>
#include <thread>
#include <SFML/System/String.hpp>




class Blob {
 public: // you need to add this to declare which elements can be accessed outside the class
  int ident = 0;
  bool isAnchor{false};
  bool isMagnetized{false};
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


//particles blobs go here for now



int nOfBlobs = 0;
int const wSize = 1000; // window size
//Blob* blobArray = new Blob[nOfBlobs];
std::vector<Blob> blobVec(0);

//Blob (&bArray)[nOfBlobs]
void Simulation(float stepSize,std::vector<Blob>& bArray,float gAcc,int radius ) {
 int r = radius;
 sf::Vector2f center(0.0,0.0);
 int substeps = 2;
 
 //std::cout << "hello?"  << std::endl;
 for (Blob& cb : bArray){
   //std::cout << cb.pos[0]  << std::endl;
   //std::cout << cb.oldPos[0]  << std::endl;
   //std::cout << "############"  << std::endl;
   for (int j=0;j < substeps;j++) {
     
     //verlet integration
     if (!cb.isAnchor) {
      cb.vel = cb.pos - cb.oldPos;
     }
     else {
      cb.vel = sf::Vector2f(0,0);
     }
     
     
     
     
     cb.oldPos = cb.pos;
     //std::cout << blobVec[nOfBlobs-1].oldPos.y << blobVec[nOfBlobs-1].pos.y << std::endl;
     //std::cout << cb.oldPos.y << cb.pos.y << std::endl;
     
     //std::cout << cb.acc.x << " " << cb.acc.y << std::endl;
     float dtCalc = pow((stepSize/substeps),2);
     cb.pos = cb.pos + cb.vel + cb.acc*dtCalc;
     
     
     


     //std::cout << "#################" << std::endl;

     //clear acceleration
     cb.acc = sf::Vector2f(0,0);
     //add gravity
     if (!cb.isAnchor) {
      cb.acc.y = -gAcc;
     }
     

     
     //bounding circle
     sf::Vector2f toObj(0,0);
     toObj = center - cb.pos;

     float dist = pow(pow(cb.pos.x,2) + pow(cb.pos.y,2),0.5);

     if (dist > r - cb.radius) {
      sf::Vector2f n = toObj / dist;

      cb.pos = center - n*(r-cb.radius);
      //std::cout << "NEW POS: ";
      //std::cout << cb.pos[0]  << std::endl;

     }

     //collision detection and magnetic attraction detection in one sweep loop!
     for (Blob& kb : bArray) {
      if (kb.ident != cb.ident && pow(pow(cb.pos.x-kb.pos.x,2) + pow(cb.pos.y-kb.pos.y,2),0.5) < cb.radius+kb.radius)  {
       // std::cout << "DOES HAPPEN" << std::endl;
        sf::Vector2f collAxis = cb.pos - kb.pos;

        float dist2 = pow(pow(collAxis.x,2) + pow(collAxis.y,2),0.5);
        if (dist2 < cb.radius + kb.radius) {
         sf::Vector2f m = collAxis / dist2;
         float delta = (cb.radius + kb.radius) - dist2;
         
         cb.pos = cb.pos + 0.5f*delta*m;

         kb.pos = kb.pos - 0.5f*delta*m;

        }
      }
      if (kb.ident != cb.ident && kb.isMagnetized && cb.isMagnetized) {
        //magnetic attraction basically (or gravitational lol)
        float theta = 6.2832;

        float angle1 = (std::atan2(cb.pos.y-kb.pos.y,cb.pos.x-kb.pos.y) + theta);
        angle1 = fmod(angle1,theta);
        //angle2 = (math.atan2(k.y-j.y,k.x - j.x) + 6.2832) % 6.2832
        float angle2 = (std::atan2(kb.pos.y-cb.pos.y,kb.pos.x-cb.pos.y) + theta);
        angle2 = fmod(angle2,theta);
        float gForce = 100/(pow(pow(cb.pos.x-kb.pos.x,2) + pow(cb.pos.y-kb.pos.y,2),0.5)); //30 is an arbitrary constant lol

        
        cb.acc.x += -gForce*std::cos(angle1);
        cb.acc.y += -gForce*std::sin(angle1);
        kb.acc.x += -gForce*std::cos(angle2);
        kb.acc.y += -gForce*std::sin(angle2);
        //std::cout << cb.ident << std::endl;
        //std::cout << cb.acc.x << cb.acc.y << std::endl << kb.acc.x << kb.acc.y << std::endl << "##########" << std::endl;

      }
     }

   }
 }
}

/// Freezes time so that keypresses dont last thru multiple iterations thus avoiding multiple event triggering (for example)
// left click spawns only 1 particle
void keyPressTimeFreeze() {
  std::chrono::milliseconds timespan(100); 
  std::this_thread::sleep_for(timespan);
}



int main()
{
    //basic constants
    float g = 0; //zero for now
    float dt = 0.01;
    bool magneticsCheck = false;
    double time;
    int r = 300;
    bool isInfoOn = false;
    int blobN = 0;
    bool timeFreeze = false;
    
    sf::Vector2f cameraPos = sf::Vector2f(0,0);
    
    //objects for render

    sf::RenderWindow window(sf::VideoMode(wSize, wSize), "Phys Engine V05");

    std::vector<sf::CircleShape> blobShapeVec(nOfBlobs);  // sf::CircleShape blobShapeArray[nOfBlobs];

    sf::CircleShape boundingCircle(r);
    boundingCircle.setFillColor(sf::Color::White);
    boundingCircle.setOrigin(r,r);
    boundingCircle.setPosition(wSize/2,wSize/2);
    


    //inputs
    sf::Mouse mouse; // mouse 
    sf::Keyboard keyboard;

    
    //text consts
    sf::Font standardFont;
    standardFont.loadFromFile("Raleway-Light.ttf");
    int textSize = 25;


    //texts
    sf::Text timeText;
    timeText.setCharacterSize(textSize);
    timeText.setFont(standardFont);
    timeText.setPosition(0,100);

    sf::Text fpsText;
    fpsText.setCharacterSize(textSize);
    fpsText.setFont(standardFont);
    fpsText.setPosition(0,160);

    sf::Text blobInfoText;
    blobInfoText.setCharacterSize(textSize);
    blobInfoText.setFont(standardFont);
    blobInfoText.setPosition(0,240);

    sf::Text rInfoText;
    rInfoText.setCharacterSize(textSize);
    rInfoText.setFont(standardFont);
    rInfoText.setPosition(0,350);



    while (window.isOpen()) {
     //get start time
     auto startTime = std::chrono::high_resolution_clock::now();
     

     sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
     

     //simulate next step
     if (!timeFreeze) {
      Simulation(dt,blobVec,g,r);
      time += static_cast<double>(dt);
     
      sf::String tString("Time: " + std::to_string(time));
      timeText.setString(tString);
     }
     
     //

     
     
     
     
     //#####INPUTS######

     //move camera (primary key listeners)
     bool isWClicked = keyboard.isKeyPressed(sf::Keyboard::Key::W);
     bool isAClicked = keyboard.isKeyPressed(sf::Keyboard::Key::A);
     bool isSClicked = keyboard.isKeyPressed(sf::Keyboard::Key::S);
     bool isDClicked = keyboard.isKeyPressed(sf::Keyboard::Key::D);
    
    //secondary key listeners 
     bool isLeftClicked = mouse.isButtonPressed(sf::Mouse::Button::Left);
     bool isRightClicked = mouse.isButtonPressed(sf::Mouse::Button::Right);
     bool isTClicked = keyboard.isKeyPressed(sf::Keyboard::Key::T);
     bool isMClicked = keyboard.isKeyPressed(sf::Keyboard::Key::M);
     bool isRClicked = keyboard.isKeyPressed(sf::Keyboard::Key::R);
     bool isGClicked = keyboard.isKeyPressed(sf::Keyboard::Key::G);

     //move camera without 4 if statemets
     //cameraPos.y += isWClicked*0.5;
     //cameraPos.y -= isSClicked*0.5;
     //cameraPos.x += isDClicked*0.5;
     //cameraPos.x -= isAClicked*0.5;

     if (isLeftClicked) {
      sf::Vector2i mouseCoords = mouse.getPosition(window);
      Blob newBlob;

      newBlob.pos.x = mouseCoords.x - wSize/2;
      newBlob.pos.y = mouseCoords.y - wSize/2;
      newBlob.oldPos.x = newBlob.pos.x;
      newBlob.oldPos.y = newBlob.pos.y;
      newBlob.ident = nOfBlobs;
      newBlob.color = sf::Color(0,230,255,255);
      newBlob.isMagnetized = magneticsCheck;
      if (isAClicked) {
        newBlob.isAnchor = true;
      }
      

      blobVec.resize(nOfBlobs + 1);
      blobShapeVec.resize(nOfBlobs + 1);
      blobVec[nOfBlobs] = newBlob;

      
      sf::CircleShape shape(10.f);
      shape.setFillColor(blobVec[nOfBlobs].color);
      shape.setOrigin(10.f,10.f);
      shape.setRadius(blobVec[nOfBlobs].radius);
      blobShapeVec[nOfBlobs] = shape;
       
      nOfBlobs++;
      //std::cout << nOfBlobs << std::endl;
      
      keyPressTimeFreeze();

      
     

     }
     
     
     if (isRightClicked) {

      
      sf::Vector2i mouseCoords = mouse.getPosition(window);

      for (int i=0;i<nOfBlobs;i++) {
        Blob currBlob = blobVec[i];

        float distance = pow(pow((blobVec[i].pos.x - (mouseCoords.x - wSize/2) ),2) + pow((blobVec[i].pos.y - (mouseCoords.y - wSize/2) ),2),0.5);
        //checks if we clicked on a blob 
        if (distance < 15) {
          
          isInfoOn = true;
          blobN = i; 
          
          //window.draw(blobInfoText);
        }
      }

      keyPressTimeFreeze();
     }
     
     //freeze time
     
     if (isTClicked) {
      timeFreeze = !timeFreeze;
      keyPressTimeFreeze();
     }

     //move a particle
     

     if (isRightClicked && isMClicked) {
      sf::Vector2i mouseCoords = mouse.getPosition(window);

      blobVec[blobN].pos.x = mouseCoords.x - wSize/2;
      blobVec[blobN].pos.y = mouseCoords.y - wSize/2;
      blobVec[blobN].oldPos = blobVec[blobN].pos;

     }
     
     //change particle radius dynamically
     
     if (isRightClicked && isRClicked) {
        sf::Vector2i mouseCoords = mouse.getPosition(window);
        float distance = pow(pow((blobVec[blobN].pos.x - (mouseCoords.x - wSize/2) ),2) + 
        pow((blobVec[blobN].pos.y - (mouseCoords.y - wSize/2) ),2),0.5);

        blobVec[blobN].radius += distance;
        blobShapeVec[blobN].setRadius(blobVec[blobN].radius);
        blobShapeVec[blobN].setOrigin(blobVec[blobN].radius,blobVec[blobN].radius);
        keyPressTimeFreeze();
     }

     //magnetize/unmagnetize a particle (it will be attracted by other magnetized particles)
     
     if (isGClicked) {
      for (int i=0;i<nOfBlobs;i++) {
        blobVec[i].isMagnetized = !blobVec[i].isMagnetized; 
        magneticsCheck = !magneticsCheck;
      }
      
      keyPressTimeFreeze();
     }

     
     

     //#####INPUTS######
     

     window.clear();


     //render all the static objects
     
     window.draw(boundingCircle);
     window.draw(timeText);
     if (isInfoOn) {

      //generate a display 
      sf::String blobInfoString("Blob " + std::to_string(blobVec[blobN].ident) + " Info \n" +
          "Position: " + std::to_string(blobVec[blobN].pos.x) + " " + std::to_string(blobVec[blobN].pos.y) + "\n" +
          "Velocity: " + std::to_string(blobVec[blobN].vel.x) + " " + std::to_string(blobVec[blobN].vel.y) + "\n" +
          "Color: " + std::to_string(blobVec[blobN].color.r) + " " + std::to_string(blobVec[blobN].color.g) + " " + std::to_string(blobVec[blobN].color.b)
          );
      blobInfoText.setString(blobInfoString);


      window.draw(blobInfoText);

      sf::String rInfoString("Radius: " + std::to_string(blobVec[blobN].radius) + "\n" +
      "Is Magnetic? " + std::to_string(blobVec[blobN].isMagnetized));
      rInfoText.setString(rInfoString);

      window.draw(rInfoText);
      

    
     }

     //render blobs
     for (int i=0;i<nOfBlobs;i++)
     {
       Blob currBlob = blobVec[i];
       
       
       blobShapeVec[i].setPosition(blobVec[i].pos.x + wSize/2 - cameraPos.x,blobVec[i].pos.y + wSize/2 + cameraPos.y);
       
       //std::cout << blobShapeArray[i].getPosition().y << std::endl;
       window.draw(blobShapeVec[i]);
     }
    

     

     
     //measure fps
     auto endTime = std::chrono::high_resolution_clock::now();
     auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
     float fps = 1000000.f/frameDuration.count(); 
     
     sf::String fpsString("FPS: " + std::to_string(fps));
     fpsText.setString(fpsString);

     window.draw(fpsText);
     
     window.display();

    }

    
    
    

   

    return 0;
}








