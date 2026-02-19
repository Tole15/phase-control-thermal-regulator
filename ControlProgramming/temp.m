clc;
clear;

% Configurar la comunicación serial
serialPort = 'COM8'; % Cambiar según el puerto correspondiente
baudRate = 9600;
s = serial(serialPort, 'BaudRate', baudRate, 'Terminator', 'LF');

% Abrir el puerto serial
fopen(s);

% Inicializar variables para almacenar los datos
temperatureData = [];
uData = [];
cmpData = [];
timeData = [];
startTime = datetime('now');

% Configurar la figura para la gráfica en tiempo real
figure;
h = animatedline;
ax = gca;
ax.YGrid = 'on';
ax.YLim = [0 50]; % Ajustar según el rango esperado de temperaturas
xlabel('Tiempo (s)');
ylabel('Temperatura (°C)');
title('Temperatura en Tiempo Real');

% Loop para leer y graficar los datos en tiempo real
try
    while isvalid(s)
        data = fgetl(s);
        if contains(data, 'y=')
            temperature = sscanf(data, 'y=%f');
            temperatureData(end+1) = temperature;
            timeData(end+1) = seconds(datetime('now') - startTime);
            
            % Agregar datos a la gráfica
            addpoints(h, timeData(end), temperatureData(end));
            drawnow;
        elseif contains(data, 'u=')
            u = sscanf(data, 'u=%f');
            uData(end+1) = u;
        elseif contains(data, 'CMP=')
            cmp = sscanf(data, 'CMP=%d');
            cmpData(end+1) = cmp;
        end
    end
catch ME
    fclose(s);
    delete(s);
    rethrow(ME);
end

% Cerrar el puerto serial cuando se termine el loop
fclose(s);
delete(s);

% Mostrar los datos al final
disp('Datos de Temperatura:');
disp(temperatureData);
disp('Datos de u:');
disp(uData);
disp('Datos de CMP:');
disp(cmpData);
