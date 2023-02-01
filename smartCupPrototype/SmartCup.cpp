#include "Arduino.h"
#include "SmartCup.h"

/**
 * New cup class.
 */

cup::cup() {
    float Rc = 3; // Cup radius in cm.
    float hc = 10; // Cup height in cm.
    float D_drink = 6.0; // Drinking aperature diameter in cm.
    float D_vent = 6.0; // Vent hole diameter in cm.
    float x_drink = 3.0; // Radial distance from edge to center of drinking aperature.
    float x_vent = 3.0; //  Radial distance from edge to center of vent hole.  
}

cup::cup(float r, float h, float dd, float dv, float xd, float xv) {
    Rc = r;
    hc = h;
    D_drink = dd;
    D_vent = dv;
    x_drink = xd; 
    x_vent = xv;
}

float cup::volume(){
    float volume;
    volume = PI*Rc*Rc*hc;
    return volume;
}

float cup::dr_min(float a){
    float y;
    y=hc*cos(a)+(x_drink-(D_drink/2.0))*sin(a);
    return y; 
}

float cup::dr_max(float a){
    float y;
    y=hc*cos(a)+(x_drink+(D_drink/2))*sin(a);
    return y; 
}

float cup::dr_area(){
    float a;
    a=PI*(D_drink/2)*(D_drink/2);
    return a;
} 

/**
 * New liquid class.
*/

liquid::liquid() {
  float volume = 50; // Volume in ml.
  float density = 1; // Density in g/cm^3.
  float flowFactor = 1; // Flow factor relative to water  
  float hl = 1.7684; // Height in cm of the leading edge of the liquid.
  float h0 = 1.7684; // Initial height.
  float xl = 0; // Distance of leading edge along the top.
  float ht = hl; // Height in cm of the trailing edge of the liquid.
  float xt = 0; //Distance of trailing edge along the bottom.
}  

liquid::liquid (float v, float d, float f, cup c) {  
    volume = v;
    density = d;
    flowFactor = f;
    h0_solve(c);
    hl = ht = h0;
}
  
float liquid::zdim(float angle){
    float z;
    z=hl*cos(angle)+xl*sin(angle);
    return z;
}
  
void liquid::h0_solve(cup c) {
    h0=volume/(PI*c.Rc*c.Rc);
}

    
void liquid::hl_solve(cup c, float angle) {
    float angle1 = atan(c.hc/(2*c.Rc));
    float a[] = {-0.004233,0.050257,0.115456,-0.01907};
    float ah[] = {1.0, 1.28744,0.74243,-0.02755};
    float ah0[] = {0.03872,-0.30446,0.85777, -01.02043};
    float h_in[] = {0,0,0,0,0};
    float** polyRoot;
    float ha = 0; //Track the height of the air volume from the top of the cup downward.
    float htemp=0, xtemp=0,ttemp=0;
    float xmid=c.Rc;
    float xmidtemp=0;
    float x[3] = {0,0,0};
    float y[3] = {0,0,0};
    float aa,bb,cc,dd,f,g,h,i,j,k,L,m,n,p,r,s,t,u;
    float t1,t2,t3;
    float rtTemp[4];
    float xTemp;
    
    if(hl < c.hc && ht > 0) {
        // LIQUID IS TOUCHING THE COMPLETE BOTTOM AND NOT THE TOP.
        hl=min(volume/(PI*c.Rc*c.Rc)+c.Rc*tan(angle),c.hc);
        ht = max(hl-2*c.Rc*tan(angle),0);        
    } else if((hl >= c.hc && ht > 0)||(hl < c.hc && ht <= 0)){
        // LIQUID IS TOUCHING THE COMPLETE BOTTOM AND PART OF THE TOP.
        // OR LIQUID IS TOUCHING PART OF THE BOTTOM AND NOT THE TOP.

        h_in[0] = a[0];
        h_in[1] = a[1];
        h_in[2] = a[2];
        h_in[3] = a[3];
        
        if (hl >= c.hc) {
            h_in[4] = (-c.volume()+volume)/((c.Rc*c.Rc)*tan(angle));
            xTemp=xl;    
        } else {
            h_in[4] = -volume/((c.Rc*c.Rc)*tan(angle));
            xTemp=xt;
        }
        
        polyRoot = quartic(h_in[0],h_in[1],h_in[2],h_in[3],h_in[4]);
        
        for(int cnt=0; cnt<4; cnt++){
            polyRoot[cnt][0]=polyRoot[cnt][0]-2*c.Rc+xTemp;
            polyRoot[cnt][0]=sqrt(polyRoot[cnt][0]*polyRoot[cnt][0]);            
        }
        xTemp = min(min(polyRoot[0][0],polyRoot[1][0]),
              min(polyRoot[2][0],polyRoot[3][0]))+xTemp;

        if (hl >= c.hc) {
            xl=xTemp;
            ht = max((c.hc-(2*c.Rc-xl)*tan(angle)),0);
            hl = c.hc;
        } else {
            xt=xTemp;
            ht = 0;
            hl = min((2*c.Rc-xt)*tan(angle),c.hc);
        }
    } else if ((2*c.Rc-xt)-xl >.0001){
        // LIQUID IS ALMOST HORIZONTAL TOUCHING PART OF THE BOTTOM AND PART OF THE TOP.
        xt = hl_solve_truncwedge(c, angle);
        hl = c.hc;
        xl = 2*c.Rc - xt - (c.hc/tan(angle));
        ht = 0;
    } else {
        // LIQUID IS HORIZONTAL TOUCHING PART OF THE BOTTOM AND PART OF THE TOP.
        aa = (ah0[0]*c.Rc*c.Rc*c.Rc+ah0[1]*c.Rc*c.Rc+ah0[2]*c.Rc+ah0[3])*c.hc;
        bb = ah[1]*c.hc;
        cc = ah[2]*c.Rc*c.hc;
        dd = ah[3]*c.Rc*c.Rc*c.hc-volume; 

        xmid = xl/2+c.Rc-xt/2;
        
        polyRoot = cubic(aa, bb, cc, dd);

        if(polyRoot[0][0] > 0.0 && polyRoot[0][0] <=c.Rc) {
          xmid = polyRoot[0][0];
        } else if(polyRoot[1][0] > 0.0 && polyRoot[1][0] <=c.Rc) {
          xmid = polyRoot[1][0];
        } else {  
          xmid = polyRoot[2][0];
        }
        hl = c.hc;
        xl = xmid;
        ht = 0;
        xt = 2*c.Rc-xmid;
      }
} 

float ** liquid::cubic(float ain, float bin, float cin, float din) {
    float** cubeRoot =0;
    cubeRoot = new float*[3];
    for (int x = 0; x<3; x++) {
        cubeRoot[x] = new float[2];
    }
    float f,g,h,i,j,k,L,m,n,p,r,s,t,u;
    float t1,t2,t3;

    f = (3*cin/ain-(bin*bin/(ain*ain)))/3;
    g = ((2*pow(bin,3)/(1*pow(ain,3)))-(9*bin*cin/(ain*ain))+27*din/ain)/27;
    h = g*g/4+f*f*f/27;
    
    if(f==0 && g==0 && h==0)
    {
        // real roots and equal
        cubeRoot[0][0]=-1*pow((din/ain),1/3.0);
        cubeRoot[2][0]=cubeRoot[1][0]=cubeRoot[0][0];    
    } else if(h<=0) {
        // 3 real roots
        i = sqrt(g*g/4-h);
        j = pow(i, 1/3.0);
        k = acos(-g/(2*i));
        L = -j;
        m = cos(k/3);
        n = sqrt(3)*sin(k/3);
        p = -bin/(3*ain);
        cubeRoot[0][0] = 2*j*m+p;
        cubeRoot[0][1] = 0.0;
        cubeRoot[1][0] = (L*(m+n)+p);
        cubeRoot[1][1] = 0.0;
        cubeRoot[2][0] = (L*(m-n)+p);
        cubeRoot[2][1] = 0.0;

    } else {
        // one real root and two Complex roots.
        r = sqrt(h)-g/2;
        s = r/fabs(r)*pow(fabs(r),1/3.0);
        t = -1*sqrt(h)-g/2;
        u = t/fabs(t)*pow(fabs(t),1/3.0);
        t1=s+u+p;
        t2=-(s+u)/2+p;
        t3=(s-u)*sqrt(3)/2;

        cubeRoot[0][0]=t1;
        cubeRoot[0][1]=0.0;
        cubeRoot[1][0]=t2;
        cubeRoot[1][1]=t3;
        cubeRoot[2][0]=t2;
        cubeRoot[2][1]=-t3;
    }
  return cubeRoot;    
}

float * liquid::comroot(float Re, float Im) {
    float * sqout;
    float modulus = Re*Re + Im*Im;
    float rr = sqrt(modulus);
    float y = sqrt((rr-Re)/2);
    float x = Im/(2*y);
    if(x<0) {
       sqout[0]=-x;
       sqout[1]=-y;
    }
    return sqout;
}

float ** liquid::quartic(float aq, float bq, float cq, float dq, float eq) {
    float** out =0;
    out = new float*[4];
    for (int x = 0; x<3; x++) {
        out[x] = new float[2];
        out[x][1]=0.0;
    }
    float k[4] = {0,0,0,0};
    float f,g,h;
    float p,q,r,s,z,z2;
    float** Y;
    float* proot;
    float* qroot;
    float* rroot;
    float inR, inI;
    
      bq = bq/aq;
      cq = cq/aq;
      dq = dq/aq;
      eq = eq/aq;
      
      f = cq-0.375*bq*bq;
      g = dq+pow(bq,3)/8-0.5*bq*cq;
      h = eq-(3*pow(bq,4))/256+0.0625*bq*bq*cq-0.25*bq*dq;
      
      k[3] = 1;
      k[2] = f/2;
      k[1] = (f*f-4*h)/16;
      k[0] = -g*g/64;
      
      Y = cubic(k[3],k[2],k[1],k[0]);
      
      // Check if the first two roots are real roots using a threshold 
      //of +/- .0001 for the imaginery part.
      if(fabs(Y[0][1]) <0.0001 && fabs(Y[1][1]) <0.0001) {
        // Check if the first root is non-zero.
        if(fabs(Y[0][0]) >= 0.0001) {
          p = sqrt(Y[0][0]);
          // check if the second root is non-zero.
          if(fabs(Y[1][0]) >= 0.0001) {
            q = sqrt(Y[1][0]);
          } else {
            q = sqrt(Y[2][0]);
          }
        } else {
          // Case for first root being zero. Set other two roots to p and q.
          p = sqrt(Y[1][0]);
          q = sqrt(Y[2][0]);
        }
       
        r = -g/(8*p*q);
        s = bq/4;
        out[0][0]=p+q+r-s;
        out[1][0]=p-q-r-s;
        out[2][0]=-p+q-r-s;
        out[3][0]=-p-q+r-s;
      } else {
        // Case when roots are complex.
        // Check if the first root is complex.
        if(abs(Y[0][1]) > 0.0001) {
            inR = Y[0][0];
            inI = Y[0][1];
            rroot = comroot(inR, inI);
          // Check if the second root is complex.
          if(abs(Y[1][1]) > 0.0001) {
            inR = Y[1][0];
            inI = Y[1][1];
            qroot = comroot(inR, inI);
          } else {
            inR = Y[2][0];
            inI = Y[2][1];
            qroot = comroot(inR, inI);
          }
        } else {
          // Case when the second and third root are complex.
            inR = Y[1][0];
            inI = Y[1][1];
            rroot = comroot(inR, inI);
            inR = Y[2][0];
            inI = Y[2][1];
            qroot = comroot(inR, inI);
        }
       
        r = -g/(8*(rroot[0]*qroot[0]-rroot[1]*qroot[1]));
        s = bq/4;
        out[0][0]=rroot[0]+qroot[0]+r-s;
        out[1][0]=rroot[0]-qroot[0]-r-s;
        out[2][0]=-rroot[0]+qroot[0]-r-s;
        out[3][0]=-rroot[0]-qroot[0]+r-s;
        out[0][1]=rroot[1]+qroot[1];
        out[1][1]=rroot[1]-qroot[1];
        out[2][1]=-rroot[1]+qroot[1];
        out[3][1]=-rroot[1]-qroot[1];
      }
      return out;    
}

float liquid::hl_solve_truncwedge(cup c, float angle) {
    float a[] = {-0.004233,0.050257,0.115456,-0.01907};
    float D[] = {0,0,0,0}; // Coeeficients for the total wedge.
    float E[] = {0,0,0,0}; // Coefficients for the virtual wedge.
    float F[] = {0,0,0,0}; // Coefficients for subtracting volumes.
    float rhtan = 2*c.Rc-(c.hc/tan(angle));
    float h_in[] = {0,0,0,0,0};
    float trncRoot[4][2];
    float xttemp=0;
    //Complex out[3];
    float x[3] = {0,0,0};
    float y[3] = {0,0,0};
    float aa,bb,cc,dd,f,g,h,i,j,k,L,m,n,p,r,s,t,u;
    float t1,t2,t3;
    
    // Assign coefficients for the total wedge.
    D[0] = -a[0];
    D[1] = 6*a[0]*c.Rc+a[1];
    D[2] = -12*a[0]*c.Rc*c.Rc-4*a[1]*c.Rc-a[2];
    D[3] = 8*a[0]*c.Rc*c.Rc*c.Rc+4*a[1]*c.Rc*c.Rc+2*a[2]*c.Rc+a[3];
        
    // Assign coefficients for the total wedge.
    E[0] = -a[0];
    E[1] = 3*a[0]*rhtan+a[1];
    E[2] = -3*a[0]*rhtan*rhtan-2*a[1]*rhtan-a[2];
    E[3] = a[0]*rhtan*rhtan*rhtan+a[1]*rhtan*rhtan+a[2]*rhtan+a[3];
    
    // Assign coefficients for the total wedge.
    F[0] = 2*c.Rc*tan(angle);
    F[1] = -tan(angle);
    F[2] = c.hc-2.0*c.Rc*tan(angle);
    F[3] = tan(angle);
    
    // LIQUID IS TOUCHING PART OF THE BOTTOM AND NOT THE TOP.
    h_in[0] = F[1]*D[0]+F[3]*E[0];
    aa = h_in[1] = F[0]*D[0]+F[1]*D[1]+F[2]*E[0]+F[3]*E[1];
    bb = h_in[2] = F[0]*D[1]+F[1]*D[2]+F[2]*E[1]+F[3]*E[2];
    cc = h_in[3] = F[0]*D[2]+F[1]*D[3]+F[2]*E[2]+F[3]*E[3];
    dd = h_in[4] = F[0]*D[3]+F[2]*E[3]-volume/(c.Rc*c.Rc);
    //ptr = cubic(h_in[1],h_in[2],h_in[3],h_in[4]);
    f = (3*cc/aa-(bb*bb/(aa*aa)))/3;
    g = ((2*pow(bb,3)/(1*pow(aa,3)))-(9*bb*cc/(aa*aa))+27*dd/aa)/27;
    h = g*g/4+f*f*f/27;
    if(f==0 && g==0 && h==0)
    {
        // real roots and equal
        trncRoot[2][0] = trncRoot[1][0] = trncRoot[0][0] =-1*pow((dd/aa),1/3.0);
        trncRoot[2][1] = trncRoot[1][1] = trncRoot[0][1] = 0 ;
        
    } else if(h<=0) {
        // 3 real roots
        i = sqrt(g*g/4-h);
        j = pow(i, 1/3.0);
        k = acos(-g/(2*i));
        L = -j;
        m = cos(k/3);
        n = sqrt(3)*sin(k/3);
        p = -bb/(3*aa);
        trncRoot[0][0] =2*j*m+p;
        trncRoot[1][0] =(L*(m+n)+p);
        trncRoot[2][0] =(L*(m-n)+p);
        trncRoot[2][1] = trncRoot[1][1] = trncRoot[0][1] = 0 ;
    } else {
        // one real root and two Complex roots.
        r = sqrt(h)-g/2;
        s = r/abs(r)*pow(abs(r),1/3.0);
        t = -1*sqrt(h)-g/2;
        u = t/abs(t)*pow(abs(t),1/3.0);
        t1=s+u+p;
        t2=-(s+u)/2-bb/(3*aa);
        t3=(s-u)*sqrt(3)/2;
        trncRoot[0][0] =t1;
        trncRoot[0][1] =0;
        trncRoot[1][0] =t2;
        trncRoot[1][1] =t3;
        trncRoot[2][0] =t2;
        trncRoot[2][1] =-t3;
    }
    xttemp = min(abs(trncRoot[1][0]-xt),min(abs(trncRoot[0][0]-xt),abs(trncRoot[1][0]-xt)))+xt;
    return xttemp;
}
