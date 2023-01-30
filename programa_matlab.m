clc; clear; close all;

puertoserial = serialport("COM7",115200);
muestras = 50000;
i = 0;

senales = cell(1, 6); 
for k = 1:6
    senales{k} = zeros(1, muestras);
end

% Lectura 
canal = 1;
valor_n = 1;

% Figura
title('ADC', 'Interpreter', 'latex', 'FontName','Times New Roman','FontSize', 16);
xlabel('Tiempo','Interpreter', 'latex','FontName','Times New Roman','FontSize',12);
ylabel('Voltaje', 'Interpreter', 'latex', 'FontSize', 12);
grid on; 
hold on;

tiempo = 1:1:muestras;
ylim([0 3.5]);
xlim([0 muestras]);


adc0 = animatedline('Color', '#FF8000 ');
adc1 = animatedline('Color', '#80FF00');
adc2 = animatedline('Color', '#FF0080');
adc3 = animatedline('Color', '#8000000');
adc4 = animatedline('Color', '#808000');
adc5 = animatedline('Color', '#C82A54');


while true
    senales{1}(valor_n) = str2double(readline(puertoserial));
    senales{2}(valor_n) = str2double(readline(puertoserial));
    senales{3}(valor_n) = str2double(readline(puertoserial));
    senales{4}(valor_n) = str2double(readline(puertoserial));
    senales{5}(valor_n) = str2double(readline(puertoserial));
    senales{6}(valor_n) = str2double(readline(puertoserial));
    
    v_adc0 = (senales{1}(valor_n) * 3.3)/ 4095;
    v_adc1 = (senales{2}(valor_n) * 3.3)/ 4095;
    v_adc2 = (senales{3}(valor_n) * 3.3)/ 4095;
    v_adc3 = (senales{4}(valor_n) * 3.3)/ 4095;
    v_adc4 = (senales{5}(valor_n) * 3.3)/ 4095;
    v_adc5 = (senales{6}(valor_n) * 3.3)/ 4095;

    addpoints(adc0, tiempo(valor_n), v_adc0);
    addpoints(adc1, tiempo(valor_n), v_adc1);
    addpoints(adc2, tiempo(valor_n), v_adc2); 
    addpoints(adc3, tiempo(valor_n), v_adc3);
    addpoints(adc4, tiempo(valor_n), v_adc4);
    addpoints(adc5, tiempo(valor_n), v_adc5);
    drawnow

    valor_n = valor_n + 1;
    if valor_n == muestras
        break
    end
    
end

