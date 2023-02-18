#include <iostream>
#include <cmath>

const int N = 1000; // number of samples
const int L = 10;   // length of filter

int main()
{
    // generate input signal (heart beat)
    double x[N];
    for (int n = 0; n < N; n++) {
        x[n] = 0.8 * sin(2 * M_PI * 60 * n / 360) + 0.5 * sin(2 * M_PI * 80 * n / 360);
    }
    
    // generate noise signal (power line interference)
    double v[N];
    for (int n = 0; n < N; n++) {
        v[n] = 0.5 * sin(2 * M_PI * 50 * n / 360);
    }
    
    // add noise to input signal
    double d[N];
    for (int n = 0; n < N; n++) {
        d[n] = x[n] + v[n];
    }
    
    // initialize filter coefficients to zero
    double w[L] = {0};
    
    // initialize step size and forgetting factor
    double mu = 0.1;
    double lambda = 0.99;
    
    // initialize past inputs and outputs
    double u[L] = {0};
    double y[L] = {0};
    
    // adaptive filtering
    double e[N] = {0};
    for (int n = 0; n < N; n++) {
        // update past inputs and outputs
        for (int i = L - 1; i >= 1; i--) {
            u[i] = u[i-1];
            y[i] = y[i-1];
        }
        u[0] = d[n];
        y[0] = 0;
        for (int i = 0; i < L; i++) {
            y[0] += w[i] * u[i];
        }
        
        // update filter coefficients
        double d_hat = y[0];
        double error = x[n] - d_hat;
        for (int i = 0; i < L; i++) {
            w[i] = lambda * w[i] + mu * error * u[i];
        }
        
        // compute filtered output
        double y_hat = 0;
        for (int i = 0; i < L; i++) {
            y_hat += w[i] * u[i];
        }
        
        // compute error signal
        e[n] = x[n] - y_hat;
    }
    
    // print results
    for (int n = 0; n < N; n++) {
        std::cout << e[n] << std::endl;
    }
    
    return 0;
}
// Explanation is given in ss