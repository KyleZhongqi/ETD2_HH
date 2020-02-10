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
%V=[t;V1;V2;...], ras=[spike time;neuron index]
%CM coupling strength matrix
%CM_ij: coupling strength from j-th neuorn to i-th neuron 

 
 
 
%%
figure()
plot(V(1,:),V(2,:),'b'),hold on
plot(V(1,:),V(3,:),'r'),hold on
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 




















