/*
    策略头文件
*/

// 每次传入一个机器人的 id 传入 当前状态
// 新的问题 同步


/*
    反向视角规避同步：
        由工作台寻找 最近的 机器人，将任务加入机器人 的工作队列
        每个机器人有一个  job_queue  pair <工作台编号，vector<int>材料列表 > 

*/

/*
    前进 / 后退
    旋转
    购买 / 出售

    机器人从 当前位置 移动至 目标位置：
        1. 原地旋转
        2. 前进
    
    购买 / 出售

    * 各个目标位置（工作台） 是 没有体积 的坐标
    机器人需要接受到的是 一连串的 坐标信息

    机器人有体积    只能携带一个物品    机器人圆心坐标到 目标点 距离小于 0.4 视为抵达
*/


/*
        BUY     SOLD    Profit          Time
    1   3K      6K      3K              50
    2   4.4K    7.6K    3.2K            50
    3   5.8K    9.2K    3.4K            50

    4   15.4K   22.5K   7.1K    1+2     500
    5   17.2K   25K     7.8K    1+3     500
    6   19.2K   27.5K   8.3K    2+3     500

    7   76K     105K    29K             1000

    4号需要 1+2 合成 当 1、2卖给可以生产4的工作台 而后再买
*/

/*
    时间规划问题

        成品格子：每个成品格子只能放一个，不能堆料

        速度问题：
            - 空载：速度尽量快
            - 载有物品：

*/

/*
    路径规划问题

        碰撞发生位置：
            - Situation 1: 行进中，路径相遇

            - Situation 2: 在工作台交接时发生碰撞
                Sol-01: 四个机器人固定每个机器人抵达工作台的 方位 四个方位
                        两步骤： 第一步走到工作台对应四个方位举例 2m（举例）处
                                第二步前进 1.5m 抵达工作台
*/

/*
    Get Speed: 
*/
double rotate();
double getSpeed(int robot_id, double st_x, double st_y, double ed_x, double ed_y);


/*
    策略 01 简单：
        1 号    
            动作1:  4工作台买 => 7工作台卖
            动作2： 1工作台买 => 4工作台卖
            动作3： 1工作台买 => 5工作台卖   
        2 号  
            动作1： 5工作台买 => 7工作台卖
            动作2： 2工作台买 => 4工作台卖
            动作3： 2工作台买 => 6工作台卖   
        3 号
            动作1： 6工作台买 => 7工作台卖
            动作2： 3工作台买 => 5工作台卖
            动作3： 3工作台买 => 6工作台卖
            
        4 号
            动作1： 7工作台买 => 8工作台卖
            动作2： 1工作台买 => 8工作台卖
            动作3： 2工作台买 => 8工作台卖
            动作4:  3工作台买 => 8工作台卖
*/


/*
    初始化の五秒钟：抽象成图
        节点和路径  转化为网络流问题？

*/