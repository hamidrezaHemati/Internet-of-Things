import json
import csv
import matplotlib.pyplot as plt
dataFile = 'book1.csv'


def get_data_files():
    files = []
    with open(dataFile) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            files.append(row)
            # print(f'{", ".join(row)}')
    return files


def open_file(data_file_name):
    f = open(data_file_name, )
    data = json.load(f)
    f.close()
    return data


def get_system_characteristics(list):
    numberOfNodes = 0
    initialLoRaSF = 0
    initialLoRaBW = 0
    initialLoRaTP = 0
    for l in list:
        for k, v in l.items():
            if k == '**.numberOfNodes':
                numberOfNodes = v
            elif k == "**.loRaNodes[*].**initialLoRaSF":
                initialLoRaSF = v
            elif k == "**.loRaNodes[*].**initialLoRaBW":
                initialLoRaBW = v
            elif k == "**.loRaNodes[*].**initialLoRaTP":
                initialLoRaTP = v
    return numberOfNodes, initialLoRaSF, initialLoRaTP, initialLoRaBW


def get_node_data(scalers):
    total_sent_packets = 0
    total_energy_consumed = 0
    totalReceivedPackets = 0
    for line in scalers:
        if line["name"] == "sentPackets":
            total_sent_packets += line["value"]
        if line["name"] == "totalEnergyConsumed":
            total_energy_consumed += line["value"]
        if line["name"] == "totalReceivedPackets":
            totalReceivedPackets = line["value"]
    return total_sent_packets, total_energy_consumed, totalReceivedPackets


def extract_data(data_file_name):
    data = open_file(data_file_name)
    total_sent_packets = 0
    total_energy_consumed = 0
    initialLoRaSF = 0
    initialLoRaTP = 0
    initialLoRaBW = 0
    totalReceivedPackets = 0
    for key1, value1 in data.items():
        for k, v in value1.items():
            print(k)
            if type(v) is list and k == 'moduleparams':
                numberOfNodes, initialLoRaSF, initialLoRaTP, initialLoRaBW = get_system_characteristics(v)
            if type(v) is list and k == 'scalars':
                total_sent_packets, total_energy_consumed, totalReceivedPackets = get_node_data(v)
    return total_sent_packets, total_energy_consumed, totalReceivedPackets, \
           initialLoRaSF, initialLoRaTP, initialLoRaBW


def plot(x, y, title, y_lable):
    plt.plot(x, y, 'ro')
    plt.title(title)
    plt.xlabel('number of nodes')
    plt.ylabel(y_lable)
    plt.show()


def title_maker(initialLoRaSF, initialLoRaTP, initialLoRaBW ):
    return str(initialLoRaSF) + "_" + str(initialLoRaTP) + "_" + str(initialLoRaBW)


def main():
    data = get_data_files()
    x = [15, 30]
    for d in data:
        energy = []
        received_ratio = []
        energy_consumption15 = 0
        energy_consumption30 = 0
        total_sent_packets15, total_energy_consumed15, totalReceivedPackets15, initialLoRaSF, initialLoRaTP, initialLoRaBW = extract_data(d[0])
        total_sent_packets30, total_energy_consumed30, totalReceivedPackets30, initialLoRaSF, initialLoRaTP, initialLoRaBW = extract_data(d[1])
        received_packets_ratio15 = totalReceivedPackets15/total_sent_packets15
        received_packets_ratio30 = totalReceivedPackets30/total_sent_packets30
        if totalReceivedPackets15 or totalReceivedPackets30 != 0:
            energy_consumption15 = total_energy_consumed15/totalReceivedPackets15
            energy_consumption30 = total_energy_consumed30/totalReceivedPackets30
        received_ratio.append(received_packets_ratio15)
        received_ratio.append(received_packets_ratio30)
        energy.append(energy_consumption15)
        energy.append(energy_consumption30)
        plot(x, received_ratio, title_maker(initialLoRaSF, initialLoRaTP, initialLoRaBW ), "Received_packets_ratio")
        plot(x, energy, title_maker(initialLoRaSF, initialLoRaTP, initialLoRaBW ), "Energy Consumption")


main()