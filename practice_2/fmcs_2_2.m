% Входные данные
P_tx_UE = 24;      % Мощность передатчика UE в дБм (UL)
P_tx_BS = 46;      % Мощность передатчика BS в дБм (DL)
G_tx_UE = 0;       % Усиление антенны UE в дБи (UL)
G_tx_BS = 21;      % Усиление антенны BS в дБи (DL и UL)
G_rx_UE = 0;       % Усиление антенны UE в дБи (DL)
G_rx_BS = 21;      % Усиление антенны BS в дБи (UL)
L_pen = 15;        % Потери на проникновение через стены в дБ
L_int = 1;         % Потери на интерференцию в дБ
N_f_BS = 2.4;      % Коэффициент шума приемника BS в дБ
N_f_UE = 6;        % Коэффициент шума приемника UE в дБ
SINR_UL = 4;       % Требуемое SINR для UL в дБ
SINR_DL = 2;       % Требуемое SINR для DL в дБ
BW_UL = 10e6;      % Полоса частот в UL (10 МГц)
BW_DL = 20e6;      % Полоса частот в DL (20 МГц)

% Чувствительность приемника BS (UL)
P_rx_BS = -174 + 10 * log10(BW_UL) + N_f_BS + SINR_UL;

% Чувствительность приемника UE (DL)
P_rx_UE = -174 + 10 * log10(BW_DL) + N_f_UE + SINR_DL;

% Расчет MAPL для UL
MAPL_UL = P_tx_UE + G_tx_UE + G_rx_BS - L_pen - L_int - P_rx_BS;

% Расчет MAPL для DL
MAPL_DL = P_tx_BS + G_tx_BS + G_rx_UE - L_pen - L_int - P_rx_UE;

% Вывод результатов
fprintf('MAPL для восходящего канала (UL): %.2f дБ\n', MAPL_UL);
fprintf('MAPL для нисходящего канала (DL): %.2f дБ\n', MAPL_DL);
