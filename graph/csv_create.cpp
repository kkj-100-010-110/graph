#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Connection
{
    std::string station_a;
    std::string station_b;
    double minutes;
    std::string line;
    std::string key;
};

int main()
{
    // Zone 1 데이터 정의
    std::vector<Connection> connections = {
        {"Paddington", "Edgware Road(Bakerloo)", 11, "Bakerloo", "B1"},
        {"Edgware Road(Bakerloo)", "Marylebone", 7, "Bakerloo", "B2"},
        {"Marylebone", "Baker Street", 6, "Bakerloo", "B3"},
        {"Baker Street", "Regent's Park", 10, "Bakerloo", "B4"},
        {"Regent's Park", "Oxford Circus", 15, "Bakerloo", "B5"},
        {"Oxford Circus", "Piccadilly Circus", 12, "Bakerloo", "B6"},
        {"Piccadilly Circus", "Charing Cross", 11, "Bakerloo", "B7"},
        {"Charing Cross", "Embankment", 3, "Bakerloo", "B8"},
        {"Embankment", "Waterloo", 6, "Bakerloo", "B9"},
        {"Waterloo", "Lambeth North", 9, "Bakerloo", "B10"},
        {"Lambeth North", "Elephant & Castle", 18, "Bakerloo", "B11"},
        {"Notting Hill Gate", "Queensway", 8, "Central", "CE1"},
        {"Queensway", "Lancaster Gate", 10, "Central", "CE2"},
        {"Lancaster Gate", "Marble Arch", 15, "Central", "CE3"},
        {"Marble Arch", "Bond Street", 7, "Central", "CE4"},
        {"Bond Street", "Oxford Circus", 7, "Central", "CE5"},
        {"Oxford Circus", "Tottenham Court Road", 9, "Central", "CE6"},
        {"Tottenham Court Road", "Holborn", 10, "Central", "CE7"},
        {"Holborn", "Chancery Lane", 10, "Central", "CE8"},
        {"Chancery Lane", "St. Paul's", 14, "Central", "CE9"},
        {"St. Paul's", "Bank", 9, "Central", "CE10"},
        {"Bank", "Liverpool Street", 10, "Central", "CE11"},
        {"Baker Street", "Bond Street", 16, "Jubilee", "J1"},
        {"Bond Street", "Green Park", 14, "Jubilee", "J2"},
        {"Green Park", "Westminster", 21, "Jubilee", "J3"},
        {"Westminster", "Waterloo", 17, "Jubilee", "J4"},
        {"Waterloo", "Southwark", 8, "Jubilee", "J5"},
        {"Southwark", "London Bridge", 19, "Jubilee", "J6"},
        {"Edgware Road", "Paddington", 10, "District", "D1"},
        {"Paddington", "Bayswater", 17, "District", "D2"},
        {"Bayswater", "Notting Hill Gate", 10, "District", "D3"},
        {"Notting Hill Gate", "High Street Kensington", 13, "District", "D4"},
        {"High Street Kensington", "Gloucester Road", 18, "District", "D5"},
        {"Earl's Court", "Gloucester Road", 12, "District", "D6"},
        {"Gloucester Road", "South Kensington", 8, "District", "D7"},
        {"South Kensington", "Sloane Square", 17, "District", "D8"},
        {"Sloane Square", "Victoria", 13, "District", "D9"},
        {"Victoria", "St James's Park", 11, "District", "D10"},
        {"St James's Park", "Westminster", 11, "District", "D11"},
        {"Westminster", "Embankment", 10, "District", "D12"},
        {"Embankment", "Temple", 9, "District", "D13"},
        {"Temple", "Blackfriars", 10, "District", "D14"},
        {"Blackfriars", "Mansion House", 10, "District", "D15"},
        {"Mansion House", "Cannon Street", 4, "District", "D16"},
        {"Cannon Street", "Monument", 5, "District", "D17"},
        {"Monument", "Tower Hill", 10, "District", "D18"},
        {"Tower Hill", "Aldgate East", 10, "District", "D19"},
        {"Edgware Road", "Paddington", 10, "Circle", "CI1"},
        {"Paddington", "Bayswater", 17, "Circle", "CI2"},
        {"Bayswater", "Notting Hill Gate", 10, "Circle", "CI3"},
        {"Notting Hill Gate", "High Street Kensington", 13, "Circle", "CI4"},
        {"High Street Kensington", "Gloucester Road", 12, "Circle", "CI5"},
        {"Gloucester Road", "South Kensington", 8, "Circle", "CI6"},
        {"South Kensington", "Sloane Square", 17, "Circle", "CI7"},
        {"Sloane Square", "Victoria", 13, "Circle", "CI8"},
        {"Victoria", "St James's Park", 11, "Circle", "CI9"},
        {"St James's Park", "Westminster", 11, "Circle", "CI10"},
        {"Westminster", "Embankment", 10, "Circle", "CI11"},
        {"Embankment", "Temple", 9, "Circle", "CI12"},
        {"Temple", "Blackfriars", 10, "Circle", "CI13"},
        {"Blackfriars", "Mansion House", 10, "Circle", "CI14"},
        {"Mansion House", "Cannon Street", 4, "Circle", "CI15"},
        {"Cannon Street", "Monument", 5, "Circle", "CI16"},
        {"Monument", "Tower Hill", 10, "Circle", "CI17"},
        {"Tower Hill", "Aldgate", 9, "Circle", "CI18"},
        {"Aldgate", "Liverpool Street", 9, "Circle", "CI19"},
        {"Liverpool Street", "Moorgate", 6, "Circle", "CI20"},
        {"Moorgate", "Barbican", 10, "Circle", "CI21"},
        {"Barbican", "Farringdon", 8, "Circle", "CI22"},
        {"Farringdon", "King's Cross St. Pancras", 26, "Circle", "CI23"},
        {"King's Cross St. Pancras", "Euston Square", 15, "Circle", "CI24"},
        {"Euston Square", "Great Portland Street", 10, "Circle", "CI25"},
        {"Great Portland Street", "Baker Street", 13, "Circle", "CI26"},
        {"Baker Street", "Edgware Road", 10, "Circle", "CI27"},
        {"Aldgate", "Liverpool Street", 9, "Metropolitan", "M1"},
        {"Liverpool Street", "Moorgate", 6, "Metropolitan", "M2"},
        {"Moorgate", "Barbican", 10, "Metropolitan", "M3"},
        {"Barbican", "Farringdon", 8, "Metropolitan", "M4"},
        {"Farringdon", "King's Cross St. Pancras", 26, "Metropolitan", "M5"},
        {"King's Cross St. Pancras", "Euston Square", 15, "Metropolitan", "M6"},
        {"Euston Square", "Great Portland Street", 10, "Metropolitan", "M7"},
        {"Great Portland Street", "Baker Street", 13, "Metropolitan", "M8"},
        {"King's Cross St. Pancras", "Euston", 12, "Victoria", "V1"},
        {"Euston", "Warren Street", 9, "Victoria", "V2"},
        {"Warren Street", "Oxford Circus", 18, "Victoria", "V3"},
        {"Oxford Circus", "Green Park", 15, "Victoria", "V4"},
        {"Green Park", "Victoria", 19, "Victoria", "V5"},
        {"Victoria", "Pimlico", 12, "Victoria", "V6"},
        {"Pimlico", "Vauxhall", 18, "Victoria", "V7"},
        {"Earl's Court", "Gloucester Road", 12, "Piccadilly", "P1"},
        {"Gloucester Road", "South Kensington", 8, "Piccadilly", "P2"},
        {"South Kensington", "Knightsbridge", 17, "Piccadilly", "P3"},
        {"Knightsbridge", "Hyde Park Corner", 7, "Piccadilly", "P4"},
        {"Hyde Park Corner", "Green Park", 12, "Piccadilly", "P5"},
        {"Green Park", "Piccadilly Circus", 8, "Piccadilly", "P6"},
        {"Piccadilly Circus", "Leicester Square", 6, "Piccadilly", "P7"},
        {"Leicester Square", "Covent Garden", 4, "Piccadilly", "P8"},
        {"Covent Garden", "Holborn", 8, "Piccadilly", "P9"},
        {"Holborn", "Russell Square", 9, "Piccadilly", "P10"},
        {"Russell Square", "King's Cross St. Pancras", 14, "Piccadilly", "P11"},
        {"Euston", "King's Cross St. Pancras", 12, "Northern", "N1"},
        {"King's Cross St. Pancras", "Angel", 16, "Northern", "N2"},
        {"Angel", "Old Street", 20, "Northern", "N3"},
        {"Old Street", "Moorgate", 9, "Northern", "N4"},
        {"Moorgate", "Bank", 9, "Northern", "N5"},
        {"Bank", "London Bridge", 6, "Northern", "N6"},
        {"London Bridge", "Borough", 9, "Northern", "N7"},
        {"Borough", "Elephant & Castle", 13, "Northern", "N8"},
        {"Euston", "Warren Street", 9, "Northern", "N9"},
        {"Warren Street", "Goodge Street", 7, "Northern", "N10"},
        {"Goodge Street", "Tottenham Court Road", 7, "Northern", "N11"},
        {"Tottenham Court Road", "Leicester Square", 8, "Northern", "N12"},
        {"Leicester Square", "Charing Cross", 7, "Northern", "N13"},
        {"Charing Cross", "Embankment", 3, "Northern", "N14"},
        {"Embankment", "Waterloo", 6, "Northern", "N15"},
        {"Waterloo", "Bank", 33, "Waterloo & City", "W1"},
        {"Aldgate East", "Liverpool Street", 11, "Hammersmith & City", "H1"},
        {"Liverpool Street", "Moorgate", 6, "Hammersmith & City", "H2"},
        {"Moorgate", "Barbican", 10, "Hammersmith & City", "H3"},
        {"Barbican", "Farringdon", 8, "Hammersmith & City", "H4"},
        {"Farringdon", "King's Cross St. Pancras", 26, "Hammersmith & City", "H5"},
        {"King's Cross St. Pancras", "Euston Square", 15, "Hammersmith & City", "H6"},
        {"Euston Square", "Great Portland Street", 10, "Hammersmith & City", "H7"},
        {"Great Portland Street", "Baker Street", 13, "Hammersmith & City", "H8"},
        {"Baker Street", "Edgware Road", 10, "Hammersmith & City", "H9"},
        {"Edgware Road", "Paddington", 10, "Hammersmith & City", "H10"},
        // {"Test1", "Test2", 7, "Test", "TEST1"},
        // {"Test2", "Test3", 8, "Test", "TEST2"},
        // {"Test3", "Test4", 9, "Test", "TEST3"},
        // {"Test4", "Test5", 10, "Test", "TEST4"},
    };

    // CSV 파일 작성
    std::ofstream file("walking_times_between_zone1_london_underground_stations.csv");
    if (!file.is_open())
    {
        std::cerr << "파일을 열 수 없습니다!" << std::endl;
        return 1;
    }

    // CSV 헤더 작성
    file << "station_a,station_b,minutes,line\n";

    // 데이터 작성
    for (const auto& connection : connections)
    {
        file << connection.station_a << "," << connection.station_b << "," << connection.minutes << "," << connection.line << ","
             << connection.key << "\n";
        file << connection.station_b << "," << connection.station_a << "," << connection.minutes << "," << connection.line << ","
             << connection.key + "R" << "\n";
    }

    file.close();

    std::cout << "CSV 파일이 성공적으로 작성되었습니다!" << std::endl;
    return 0;
}