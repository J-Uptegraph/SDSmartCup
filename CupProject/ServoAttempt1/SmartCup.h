#ifndef SmartCup_h
#define SmartCup_h
#include "Arduino.h"
//#include "Complex.h"

/* New cup class.
	Models a cylindrical cup with a vent hole and drinking aperature.
	Class includes methods to return the volume capacity, 
	return the drinking aperature area,
	return the min and max distance from the drinking edge to the edges
	of the drinking aperature.    
 */

class cup { 
  public:
    // constructors:
    cup();
    cup(float r, float h, float dd, float dv, float xd, float xv);

    // volume method:
    float volume();

    // drinking aperature methods:
    float dr_min(float a);
    float dr_max(float a);
    float dr_area();
         
    // cup parameters:
    float Rc; // Cup radius in cm.
    float hc; // Cup height in cm.
    float D_drink; // Drinking aperature diameter in cm.
    float D_vent; // Vent hole diameter in cm.
    float x_drink; // Radial distance from edge to center of drinking aperature.
    float x_vent; //  Radial distance from edge to center of vent hole.

};

/* New liquid class.
	Models the liquid properties and instantaneous geometry.
	Geometry is specified by a vertical 2D slice through the middle 
	of the cup through the plane where drinking occurs. There are four
	parameters that specify the vertical and horizontal distance of the 
	liquid from the bottom rear corner and the top front corner of the cup.
  	Class includes methods to calculate the four geometry parameters.    
 */

class liquid {
  public:
    // constructors:
    liquid();
    liquid (float v, float d, float f, cup c);

    // Methods for calculating 2D characterization parameters:
	float zdim(float angle);
	void h0_solve(cup c);
	void hl_solve(cup c, float angle);
    float * comroot(float Re, float Im);
    float ** cubic(float ain, float bin, float cin, float din);
    float ** quartic(float aq, float bq, float cq, float dq, float eq);
	float hl_solve_truncwedge(cup c, float angle);
         
    // liquid parameters:
	float volume; // Volume in ml.
  	float density; // Density in g/cm^3.
  	float flowFactor; // Flow factor relative to water  
  	float hl; // Height in cm of the leading edge of the liquid.
  	float h0; // Initial height.
  	float xl; // Distance of leading edge along the top.
  	float ht; // Height in cm of the trailing edge of the liquid.
  	float xt; //Distance of trailing edge along the bottom.
};

#endif