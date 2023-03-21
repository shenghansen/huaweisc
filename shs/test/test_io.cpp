#include<iostream>
#include<string>
#include<math.h>
using namespace std;

//计算要转向的角度
float get_angle(float x1,float y1,float x2,float y2,float orientation){
     orientation = orientation * M_PI / 180.0;
    float beta_rad = atan2(y2 - y1, x2 - x1);
    if (beta_rad < 0)
        beta_rad += 2 * M_PI;
    // 计算A点朝向与B点连线的夹角
    double angle_rad = atan2(sin(beta_rad - orientation), cos(beta_rad - orientation));
    double angle_deg = angle_rad * 180.0 / M_PI;
    return angle_deg;
}


int main(int argc, char const *argv[]){   
    float x1=0,y1=0,x2=0,y2=0,orientation=0;
    cin>>x1>>y1>>x2>>y2>>orientation;
    cout<<get_angle(x1,y1,x2,y2,orientation)<<endl;
    return 0;
}
