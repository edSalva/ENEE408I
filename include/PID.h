//Header file for PID controller class

class PID {
private:
  double kp, ki, kd;
  double integral, prev;
  double output;
public:
  PID(double p, double i, double d) {
    kp = p;
    ki = i;
    kd = d;
    integral = prev = 0;
  }
  double update(double err) {
    double deriv;
    integral += err;
    deriv = err - prev;
    prev = err;
    output = kp * err + ki * integral + kd * deriv;
    return output;
  }
};
