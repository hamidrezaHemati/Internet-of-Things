import math
from tabulate import tabulate


# time_on_air = t_payload + t_preamble = (N_payload + N_preamble) * symbol_time
def time_on_air(sf, bw):
    PL = 14  # payload is 14 bytes
    IH = 0
    DE = 1
    CR = 1
    symbol_time = (pow(2, sf) / bw)
    t_preamble = (4.25 + 8) * symbol_time
    n_payload = 8 + max(math.ceil((8 * PL - 4 * sf + 28 + 16 - 20 * IH) / (4 * (sf - 2 * DE))) * (CR + 4), 0)
    t_payload = n_payload * symbol_time
    # print("symbol time: ", symbol_time)
    print(sf, bw)
    print("N payload = ", n_payload)
    # print("T payload = ", t_payload)
    # print("T preamble = ", t_preamble)
    return (t_payload + t_preamble)


def sensitivity(sf, bw):
    bw = bw * 1000
    NF = -174 + 6 + 10 * math.log(bw, 10)
    SNR = 31 / pow(2, sf) # mW
    SNR = 10*math.log(SNR, 10)
    return math.floor(NF + SNR)


def bit_rate(sf, bw):
    CR = 0.8
    return round((bw / pow(2, sf)) * sf * CR, 1)


def SNR(sf):
    SNR = 31 / pow(2, sf) # mW
    SNR = 10*math.log(SNR, 10)
    return SNR

# def range():


def main():
    bw = [125, 250, 500]
    sf = [x for x in range(7, 13)]
    table = []
    table.append(["SF", "BW", "BitRate", "RS", "Time_on_air", "SNR"])
    for _sf in sf:
        for _bw in bw:
            data = []
            data.append(_sf)
            data.append(str(_bw) + "KHz")
            data.append(str(bit_rate(_sf, _bw)) + "Kb/s")
            data.append(str(sensitivity(_sf, _bw)) + "dbm")
            data.append(str(time_on_air(_sf, _bw)) + "ms")
            data.append(str(SNR(_sf)) + "dbm")
            table.append(data)
    print(tabulate(table, headers='firstrow', tablefmt='fancy_grid'))


main()
