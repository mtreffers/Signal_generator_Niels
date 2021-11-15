clear all
close all

%% Parameters
timer_resolution = 16;

freq_start = 50;
freq_end = 10000;
freq_resolution = 1;

desired_freq = freq_start:freq_resolution:freq_end;

duty_cycle_start = 0.1;
duty_cycle_end = 99.9;
duty_cycle_resolution = 0.1;
duty_cycles = duty_cycle_start:duty_cycle_resolution:duty_cycle_end;

counts = (1:2^timer_resolution)';

prescalers = [1 8 64 256 1024];

fprintf("Frequency:\n\t\t\tStart: %dHz\n\t\t\tStop: %dHz\n\t\t\tResolution: %dHz\nDuty cycle:\n\t\t\tStart: %d%%\n\t\t\tStop: %d%%\n\t\t\tResolution: %d%%\n" ,freq_start, freq_end, freq_resolution, duty_cycle_start, duty_cycle_end, duty_cycle_resolution);

%% Calculate counts
actual_freq = 16e6 ./ ( counts * prescalers);

%% Find optimal parameters
used_values = zeros(4, length(desired_freq));
duty_cycle_error = zeros(length(desired_freq), length(duty_cycles));

for i=1:length(desired_freq)
    [d, index] = min(abs(actual_freq - desired_freq(i)));
    
    [~, smallest_index] = min(d);
    
    used_values(1, i) = prescalers(smallest_index);
    used_values(2, i) = counts(index(smallest_index));
    used_values(3, i) = actual_freq(index(smallest_index), smallest_index);
    OCR1B = floor(used_values(2, i) ./ (100 ./ duty_cycles));
%     actual_duty_cycle = used_values(2, i) ./ OCR1B;
    actual_duty_cycle = 100 ./ (used_values(2, i) ./ OCR1B);
    duty_cycle_error(i, :) = actual_duty_cycle - duty_cycles;
    used_values(4, i) = max(abs(duty_cycle_error(i, :)));
end

%% Print results
fprintf("Reults:\n\t\t\tMaximum frequency error: %.2fHz\n\t\t\tMaximum duty cycle error: %.2f%%\n" ,max(used_values(3,:) - desired_freq), max(used_values(4,:)));

%% Plot values
figure(1)
sp1 = subplot(5,1,1);
plot(desired_freq, used_values(1,:));
title('Prescaler choice');
xlabel('Frequency');
ylabel('Prescaler');

sp2 = subplot(5,1,2);
plot(desired_freq, used_values(3,:));
title('Used frequency');
xlabel('Frequency');
ylabel('Frequency');

sp3 = subplot(5,1,3);
plot(desired_freq, used_values(3,:) - desired_freq);
title('Error on frequency');
xlabel('Frequency');
ylabel('Error');

sp4 = subplot(5,1,4);
plot(desired_freq, used_values(2,:));
title('Counts');
xlabel('Frequency');
ylabel('Counts');

sp5 = subplot(5,1,5);
plot(desired_freq, used_values(4,:));
title('Max error duty cycle');
xlabel('Frequency');
ylabel('Error');

linkaxes([sp1, sp2, sp3, sp4, sp5], 'x');

figure(2)
s = surf(desired_freq, duty_cycles, duty_cycle_error');
s.EdgeColor = 'none';
xlabel('Frequency');
ylabel('Desired duty cycle');
zlabel('Error');