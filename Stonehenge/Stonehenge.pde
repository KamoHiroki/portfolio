// program to reneder "stonehenge".
// ストーンヘンジを描画するプログラムです。

// 2021/06/03


void setup() {
  size(600, 600, P3D);    
  camera(25, -25, 25, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

  float fov = radians(45);  
  perspective(fov, float(width)/float(height), 1.0, 600.0);
}

void draw() {
  // set the background color to white
  background(255); 

  // render outer objects
  renderOutObject();
  
  // renger innner object
  renderInnerObject();
  
  // render center small box
  translate(-1,1);       // change a piller's position
  rotateZ(radians(45));  // rotateZ
  box(1, 1, 2);          // put a outer piller.
  
}

void renderOutObject(){
  int r = 12;          // The radius of the objects
  float angle = radians(18);   // The chenge of the angle of the outer object.
 
  int para = 0;     // The angle of the parameters of the outer objects.
 
  float rotate_Z = 0;  // Angle of the z-axis rotation of the outer object.
  float pil_trans_X = 0; // Movement of the x-coordinate of the pillar.
  float pil_trans_Y = 0; // Movement of the y-coordinate of the pillar.
  
  float top_trans_X = 0; // Movement of the x-coordinate of the pillar.
  float top_trans_Y = 0; // Movement of the y-coordinate of the pillar.
  
  float offset_top = radians(9); // offset for the top box
  
  for (float i = 0; i < 2.0*PI; i += PI/10.0 ) {
    rotate_Z = angle * para;
    pil_trans_X  = r*cos(i);
    pil_trans_Y  =r*sin(i);
    
    top_trans_X  = r*cos(i+offset_top);
    top_trans_Y  = r*sin(i+offset_top);

    // render the outer pillers
    translate(pil_trans_X, pil_trans_Y);    // change a piller's position
    rotateZ(rotate_Z);                  // rotateZ
    box(1, 1, 3);                           // put a outer piller.
    rotateZ(-rotate_Z);                 // rotateZ
    translate(-pil_trans_X, -pil_trans_Y);  // change a piller's position
    
    
    // render the outer boxes on the top of the outer pillers
    translate(top_trans_X, top_trans_Y, 2);    // change a piller's position
    rotateZ(rotate_Z+offset_top);          // rotateZ
    rotateX(radians(90));                      // rotateX
    box(1, 1, 3.5);                            // put a outer piller.
    rotateX(-radians(90));                     // -rotateX
    rotateZ(-rotate_Z-offset_top);         // -rotateZ
    translate(-top_trans_X, -top_trans_Y, -2); // change a piller's position
    
    para++;
  }
}

void renderInnerObject(){
  int r = 6;          // The radius of the pillers
  float angle = radians(30);   // The chenge of the angle of the object.
 
  int para = 0;     // The angle of the parameters of the outer objects.
 
  float rotate_Z = 0;  // Angle of the z-axis rotation of the outer object.
  float pil_trans_X = 0; // Movement of the x-coordinate of the pillar.
  float pil_trans_Y = 0; // Movement of the y-coordinate of the pillar.
  
  float top_trans_X = 0; // Movement of the x-coordinate of the pillar.
  float top_trans_Y = 0; // Movement of the y-coordinate of the pillar.
  
  float offset_top = radians(15); // offset for the top box
  
  for (float i = 0; i < (3.0/2.0)*PI; i += PI/6.0 ) {
    
      rotate_Z = angle * para;
      pil_trans_X  = r*cos(i);
      pil_trans_Y  = r*sin(i);
      
      top_trans_X  = r*cos(i+offset_top);
      top_trans_Y  = r*sin(i+offset_top);
  
      // render the pillers
      translate(pil_trans_X, pil_trans_Y);    // change a piller's position
      rotateZ(rotate_Z);                      // rotateZ
      box(1, 1, 3);                           // put a piller.
      rotateZ(-rotate_Z);                     // rotateZ
      translate(-pil_trans_X, -pil_trans_Y);  // change a piller's position
      
      // render the top box alternately
      if(para % 2 == 0){
        // render the boxes on the top of the pillers
        translate(top_trans_X, top_trans_Y, 2);    // change a top box's position
        rotateZ(rotate_Z+offset_top);              // rotateZ
        rotateX(radians(90));                     // rotateX
        box(1, 1, 3);                              // put a top box.
        rotateX(-radians(90));                     // -rotateX
        rotateZ(-rotate_Z-offset_top);             // rotateZ
        translate(-top_trans_X, -top_trans_Y, -2); // change a top box's position
      }
    para++;
  }
  
}
