ETD2_HH
======

This code is for evolving pulse-coupled Hodgkin-Huxley (HH) neuronal network. 
We introdce three second-order numerical methods, Runge-Kutta (RK), exponentional time differencing (ETD2), and adaptive ETD (AETD2) methods. 
The RK2 method should use a small time step on the order of 0.02 ms to satisfy the requirement of stability and is offerred as a banchmark.
The ETD2 method can use a large time step to improve the numerical efficiency, but it is less accurate than the RK2 method.
The AETD2 method combines the advantages of the RK2 and ETD2 methods that it can use a large time step (maximum 0.27 ms) to evolve the HH network while achieving comparable high accuracy as the RK2 method using a small time step. 
The detailed method can be found in [reference](https://github.com/KyleZhongqi/ETD2_HH#references). 

Functions:
=========

* Set the parameters and run the code in `Run_ETD2.m`

# Usage example
```
% ETD2_HH usage eaxmple.
% Addpath like
addpath('~/ETD2/');    %path for this Run_HH_model.m file

pm=[];                % a new parameter set;
pm.method = 2;        % methods,0--RK2, 1--ETD2, 2--AETD2
pm.seed = 11;         % seed to initialize
pm.t    = 1e3;        % Total run time
pm.dt   = 0.1;        % default: 0.01. Time step 
pm.NE = 80;           % default: 100. Number of Excitatory neurons.
pm.NI = 20;           % default: 0.   Number of Inhibitory neurons.
                      
pm.SEE = 0.002;        % default: 0. Strength from Ex. to Ex.
pm.SIE = 0.002;        % default: 0. Strength from Ex. to In.
pm.SEI = 0.002;        % default: 0. Strength from In. to Ex.
pm.SII = 0.002;        % default: 0. Strength from In. to In.
pm.Pc = 0.15;          % default: 0.1. Connection probability  
pm.pS  = 0;           % distribution of Strength. default:0--fixed value
                      % 1--Uniform,2--Gaussion,3--Exponentional,4--Lognormal                         
pm.Nu = 0.3;          % default: 0.1. Poisson input rate(KHz)  
pm.f = 0.06;           % default: 0.1. Poisson input strength                      
pm.Lyapunov = 0;      % default: 0. Compute the largest lyapunov exponent
pm.record_Spike = 1;  % default: 0. Record the raster
pm.record_Voltage = 0;% default: 0. Record the voltages


[V, ras, CM] = gen_HH_ETD2(pm);

% output data formulae: 
% V=[t;V1;V2;...], ras=[spike time;neuron index]
% CM coupling strength matrix
% CM_ij: coupling strength from j-th neuorn to i-th neuron 

```

References 
============
* Tian ZQK, Zhou D. Exponential time differencing algorithm for pulse-coupled hodgkin-huxley neuronal networks. arXiv preprint arXiv:1910.08724 (2019). 

* Börgers C, Nectow AR. Exponential time differencing for hodgkin–huxley-like odes. SIAM Journal on 
Scientific Computing 35 (2013) B623–B643. 

* CoxSM,MatthewsPC. Exponentialtimedifferencingforstiffsystems. JournalofComputationalPhysics(2002) 430–455. 

* Hansel D, Mato G, Meunier C, Neltner L. On numerical simulations of integrate-and-fire neural networks. 
Neural Computation 10 (1998) 467–483. 

* Shelley MJ, Tao L. Efficient and accurate time-stepping schemes for integrate-and-fire neuronal networks. 
Journal of Computational Neuroscience 11 (2001) 111–119. 

