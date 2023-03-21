#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <queue>
#include <math.h>

using namespace std;
#define PI 3.1415926535897932384626
#define MAX_ROBOT_NUM 4
#define ROBOT_DENSITY 20
#define MAX_FORWARD_SPEED 6
#define MIN_FORWARD_SPEED 2
#define MAX_ROTATE_SPEED 3.1415926535897932384626;
#define MAX_ACCERLATE_SPEED_WITHOUT_PRODUCT 250/(PI*0.45*0.45*20)
#define MAX_ACCERLATE_SPEED_WITH_PRODUCT 250/(PI*0.53*0.53*20)
#define MAX_ROTATE_ACCERLATE_SPEED_WITHOUT_PRODUCT  2*50/(PI*0.45*0.45*20*0.45*0.45)
#define MAX_ROTATE_ACCERLATE_SPEED_WITH_PRODUCT  2*50/(PI*0.53*0.53*20*0.53*0.53)


ofstream fout;
bool map_ok = false;
bool is_init=false;
size_t money=0;
int frameID=0;

bool buyed=false;

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
int* raw_material_status;//原材料格状态,使用二进制表示
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

//根据要转向的角度获取角速度
float get_angle_speed(float angle){
    if(angle>0&&angle<PI/8){
        // return PI / 2 * (1 - cos(angle));
        // return ((0.5 * (tanh(angle/2) + 1))-0.5)*MAX_ROTATE_SPEED;
        return 1.351283845031745*pow(angle,1/3)+PI/2;
    }
    else if(angle>PI/8){
        return MAX_ROTATE_SPEED;
    }
    else if(angle<0&&angle>-PI/8){
        return 1.351283845031745*pow(angle,1/3)-PI/2;
        // return (1 - exp(-angle)) / (1 + exp(-angle));
        // return ((0.5 * (tanh(angle/2) + 1))-0.5)*MAX_ROTATE_SPEED;;
    }
    else if(angle<-PI/8){
        return -MAX_ROTATE_SPEED;
    }

}

//根据要前进的距离获取线速度
float get_line_speed(float distance,float angle){
    if(angle>PI/2||angle<-PI/2){
        return 0;
    }
    else{
        return MAX_FORWARD_SPEED;
    }
}

//从某点到某点
void go_to_workstation(int robot_id,int workstation_id){
    float distance=get_distance(robot_location[robot_id][0],robot_location[robot_id][1],workstation_location[workstation_id][0],workstation_location[workstation_id][1]);
    float angle=get_angle(robot_location[robot_id][0],robot_location[robot_id][1],workstation_location[workstation_id][0],workstation_location[workstation_id][1],orientation[robot_id]);
    order(ROTATE,robot_id,get_angle_speed(angle)).printf_order();
    order(FORWARD,robot_id,get_line_speed(distance,angle)).printf_order();
}


bool readUntilOK() {
    bool first=false;
    fout=ofstream("/home/asc2022team0/huaweisc/shs/c++/log.txt",ios::app);
    if (!map_ok){
        char line[1024]={'*'};
        while (fgets(line, sizeof line, stdin)){
            if (line[0] == 'O' && line[1] == 'K') {
                fout<<"OK\n";
                map_ok = true;
                return true;
            }
            for (size_t i = 0; i < sizeof line; i++){
                if(line[i] == '.'||line[i] == 'A'||line[i] == '\n'||(line[i]<58&&line[i]>48) )
                fout<<line[i];
            }
        }
    }else{
        string line;
        getline(cin, line);
        stringstream ss(line);
        ss >> money;
        // fout<<frameID<<endl;
        // fout << money << endl;//money
        getline(cin, line);
        ss.clear(); 
        ss.str(line);
        ss>>workstation_num;
        // fout << workstation_num << endl;//workstation_num
        if(!is_init){
            //init workstation info
            workstation_type=new int[workstation_num];
            workstation_location=new float*[workstation_num];
            remaining_production_time=new int[workstation_num];
            raw_material_status=new int[workstation_num];
            product_status=new bool[workstation_num];
            for (size_t i = 0; i < workstation_num; i++){
                workstation_location[i]=new float[2];
            }
            is_init=true;
        }
        for (size_t i = 0; i < workstation_num; i++){
            getline(cin, line);
            ss.clear(); 
            ss.str(line);
            ss>>workstation_type[i]>>workstation_location[i][0]>>workstation_location[i][1]>>remaining_production_time[i]>>raw_material_status[i]>>product_status[i];
            // fout << workstation_type[i] << " 坐标：" << workstation_location[i][0] << "，" << workstation_location[i][1] << " 剩余时间：" << remaining_production_time[i] << " 原材料格状态：" << raw_material_status[i] << " 产品格状态：" << product_status[i] << endl;//workstation_info
        }
        for (size_t i = 0; i < MAX_ROBOT_NUM; i++){
            getline(cin, line);
            ss.clear(); 
            ss.str(line);
            ss>>workstation_id[i]>>product_type[i]>>time_value_coefficient[i]>>collision_value_coefficient[i]>>angle_speed[i]>>line_speed[i][0]>>line_speed[i][1]>>orientation[i]>>robot_location[i][0]>>robot_location[i][1];
            // fout << workstation_id[i] << " 携带物品：" << product_type[i] << " 时间系数：" << time_value_coefficient[i] << " 碰撞系数：" << collision_value_coefficient[i] << " 角速度：" << angle_speed[i] << " 线速度：" << line_speed[i][0] << "，" << line_speed[i][1] << " 朝向：" << orientation[i] << " 坐标：" << robot_location[i][0] << "，" << robot_location[i][1] << endl;//robot_info
        }
        
        getline(cin, line);
        if (line=="OK"){
            return true;
        }else{
            return false;
        }
    }
    return true;
}

int main() {
    remove("/home/asc2022team0/huaweisc/shs/c++/log.txt");
    readUntilOK();
    puts("OK");
    fflush(stdout);

    while (scanf("%d", &frameID) != EOF) {
        if(map_ok)
            frameID++;
        readUntilOK();
        printf("%d\n", frameID);
        //TODO: write code 
        // int lineSpeed = 0;
        // double angleSpeed = PI;
        // // if(frameID>10)
        // //     lineSpeed = 0;
        // for(int robotId = 0; robotId < 4; robotId++){
        //     printf("forward %d %d\n", robotId, lineSpeed);
        //     printf("rotate %d %f\n", robotId, angleSpeed);
        // }

        // 只取1号买到4号
        int target=-1;
        if(!buyed){
            for(int i=0;i<workstation_num;i++){
                if(workstation_type[i]==1&&product_status[i]==1){
                    target=i;
                    break;
                }
            }
            if(target!=-1){
                if(workstation_id[0]==target){
                    order(BUY,0).printf_order();
                    buyed=true;
                }else{
                    go_to_workstation(0,target);
                }
            }
        }else{
            for(int i=0;i<workstation_num;i++){
                if(workstation_type[i]==4&&raw_material_status[i]==0){
                    target=i;
                    break;
                }
            }
            if(target!=-1){
                if(workstation_id[0]==target){
                    order(SELL,0).printf_order();
                    buyed=false;
                }else{
                    go_to_workstation(0,target);
                }
            }
        }
        
        printf("OK\n");
        fflush(stdout);
    }
    fout.close();
    return 0;
}
