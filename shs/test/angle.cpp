#include <iostream>
#include <cmath>


using namespace std;

int main() {
    double max_angular_velocity, angular_acceleration, current_angular_velocity, target_angle, current_angle = 0, time_step = 0.02;
    cout << "Enter the maximum angular velocity: ";
    cin >> max_angular_velocity;
    cout << "Enter the angular acceleration: ";
    cin >> angular_acceleration;
    cout << "Enter the current angular velocity: ";
    cin >> current_angular_velocity;
    cout << "Enter the target angle: ";
    cin >> target_angle;
    

    while (abs(current_angle - target_angle) > 0.001) {
        double delta_angle = current_angular_velocity * time_step + 0.5 * angular_acceleration * time_step * time_step;
        current_angle += delta_angle;
        current_angular_velocity += angular_acceleration * time_step;
        if (current_angular_velocity > max_angular_velocity) {
            current_angular_velocity = max_angular_velocity;
        }
        if (abs(current_angle - target_angle) <= delta_angle) {
            current_angle = target_angle;
            current_angular_velocity = 0;
        }

cout << "Current angle: " << current_angle << ", Current angular velocity: " << current_angular_velocity << endl;
    }
    

    cout << "Robot reached target angle of " << target_angle << " with angular velocity of 0." << endl;
    
    return 0;
}