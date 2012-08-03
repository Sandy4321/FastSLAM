#include <iostream>

#include "compute_jacobians.h"
#include "math.h"
#include "pi_to_pi.h"

void compute_jacobians(Particle particle, vector<int> idf, MatrixXf R, \
        MatrixXf &zp, MatrixXf *Hv, MatrixXf *Hf, MatrixXf *Sf)
{
    VectorXf xv = particle.xv();

    int rows = (particle.xf()).rows();
    MatrixXf xf(rows,idf.size());    
    MatrixXf *Pf;

    for (int i=0; i<idf.size(); i++) {
        for (int r=0; r<(particle.xf()).rows(); r++) {
            xf(r,i) = (particle.xf())(r,(idf[i]));	
        }
        cout<<(particle.Pf())[0]<<endl;
        Pf[i] = (particle.Pf())[idf[i]]; //particle.Pf is a array of matrices
    }

    float dx,dy,d2,d;
    unsigned int i;
    MatrixXf HvMat(2,3);
    MatrixXf HfMat (2,2);

    for (i=0; i<idf.size(); i++) {
        dx = xf(0,i) - xv(0);
        dy = xf(1,i) - xv(1);
        d2 = pow(dx,2) + pow(dy,2);	
        d = sqrt(d2);

        //predicted observation
        zp(0,i) = d;
        zp(1,i) = atan2(dy,dx) - xv(2);
        zp(1,i) = pi_to_pi(zp(1,i));

        //Jacobian wrt vehicle states
        HvMat<< -dx/d, -dy/d,  0, 
            dy/d2, -dx/d2,-1;
        //Jacobian wrt feature states
        HfMat<<  dx/d,  dy/d,
            -dy/d2, dx/d2;
        Hv[i] = MatrixXf(HvMat);
        Hf[i] = MatrixXf(HfMat);
        //innovation covariance of 'feature observation given the vehicle'
        MatrixXf SfMat = HfMat*Pf[i]*HfMat.transpose() + R;
        Sf[i] = MatrixXf(SfMat);  
    }			
}
