#include <iostream>
#include <fstream>
using namespace std;

ofstream fout;
bool map_ok = false;
size_t money=0;
//机器人信息
int workstation_id[4] = {0};//所处工作台id
int product_type[4] = {0};//携带产品类型
double time_value_coefficient[4] = {0};//时间价值系数
double collision_value_coefficient[4] = {0};//碰撞价值系数
float angle_speed[4] = {0};//角速度
float line_speed[4][2] = {0};//线速度
float orientation[4] = {0};//朝向
float robot_location[4][2] = {0};//坐标

// 工作台信息
int workstation_num;//工作台总数
int* workstation_type;//工作台类型
float** workstation_location;//工作台坐标
int* remaining_production_time;//剩余生产时间
//TODO: 二进制表示方式
int* raw_material_status;//原材料格状态
bool* product_status;//产品格状态

enum command_type{
    FORWARD,
    ROTATE,
    BUY,
    SELL,
    DESTORY
};

class order{
private:
    command_type type;
    int robot_id;
    double value=-1000;
public:
    order(command_type type,int robot_id,double value):type(type),robot_id(robot_id),value(value){}
    order(command_type type,int robot_id):type(type),robot_id(robot_id){}
    void printf_order(){
        switch(type){
            case FORWARD:
                printf("forward");
                break;
            case ROTATE:
                printf("rotate");
                break;
            case BUY:
                printf("buy");
                break;  
            case SELL:
                printf("sell");
                break;
            case DESTORY:
                printf("destory");
                break;
        }
        if(value!=-1000)
            printf(" %d %f\n",robot_id,value);
        else
            printf(" %d\n",robot_id);
    }
};

class order_list{
private:
    queue<order> order_list;
    int next_order=INT32_MAX;
public:
    void add_order(order order){
        order_list.push(order);
    }
    void printf_order(){
        if(next_order>0){
            next_order--;
        }
        else if(next_order==0 && !order_list.empty()){
            order_list.front().printf_order();
            order_list.pop();
        }
    }    
};

//计算要转向的角度
float get_angle(float x1,float y1,float x2,float y2,float orientation){
    float beta_rad = atan2(y2 - y1, x2 - x1);
    if (beta_rad < 0)
        beta_rad += 2 * M_PI;
    // 计算A点朝向与B点连线的夹角
    double angle_rad = atan2(sin(beta_rad - orientation), cos(beta_rad - orientation));
    return angle_rad;
}

//计算两点间距离
float get_distance(float x1,float y1,float x2,float y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}




bool readUntilOK() {
    bool first=false;
    fout=ofstream("/home/asc2022team0/huaweisc/shs/c++/log.txt",ios::app);
    char line[1024]={'*'};
    while (fgets(line, sizeof line, stdin)) {
         if (!map_ok){
            for (size_t i = 0; i < sizeof line; i++){
                if(line[i] == '.'||line[i] == 'A'||line[i] == '\n'||(line[i]<58&&line[i]>48) )
                fout<<line[i];
            }
        }
        else{
            
            // for (size_t i = 0; i < sizeof line; i++){
            //     if(line[i] == ' ' || line[i] == '.'||line[i] == '-'||line[i] == '\n'||(line[i]<58&&line[i]>47) )
            //         fout<<line[i];
            // }
        }
        if (line[0] == 'O' && line[1] == 'K') {
            fout<<"OK\n";
            map_ok = true;
            return true;
        }
        fill(line, line + sizeof line, '*');
    }

    return false;
}

int main() {
    remove("/home/asc2022team0/huaweisc/shs/c++/log.txt");
    readUntilOK();
    puts("OK");
    fflush(stdout);
    int frameID=0;
    while (scanf("%d", &frameID) != EOF) {
        frameID++;
        readUntilOK();
        printf("%d\n", frameID);
        int lineSpeed = 3;
        double angleSpeed = 1.5;
        for(int robotId = 0; robotId < 4; robotId++){
            printf("forward %d %d\n", robotId, lineSpeed);
            printf("rotate %d %f\n", robotId, angleSpeed);
        }
        printf("OK\n");
        fflush(stdout);
    }
    fout.close();
    return 0;
}
