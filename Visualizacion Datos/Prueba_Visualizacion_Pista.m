% Constantes
R = 11.5; % Radio rueda
L = 200;  % Distancia entre ruedas
P = 2*pi*R/10; % Distancia de de revolucion

% Datos de encoders
encoderLeft =  [ 1000; 2000; 3000; 4000; 5000; 6000; 7000; 8250; 9500; 10750; 12000];
encoderRigth = [ 1000; 2000; 3000; 4000; 5000; 6000; 7000; 7750; 8500; 9250;  10000];

for i = 1:length(encoderLeft)
    currentTime = encoderLeft(i,1);
    if i == 1
        pastTime = 0;
    else
        pastTime = encoderLeft((i-1),1);
    end
    
    encoderLeft(i,2) = P/(currentTime - pastTime);
end

for i = 1:length(encoderRigth)
    currentTime = encoderRigth(i,1);
    if i == 1
        pastTime = 0;
    else
        pastTime = encoderRigth((i-1),1);
    end
    
    encoderRigth(i,2) = P/(currentTime - pastTime);
end

sampleTime = 250;
for i = 0:sampleTime:1000

end