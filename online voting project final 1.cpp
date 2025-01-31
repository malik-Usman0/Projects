#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

class Party {
public:
    string name;
    int votesSenate;
    int votesParliament;
    int code;
    string leader;

    Party(string n, int c, string l) : name(n), votesSenate(0), votesParliament(0), code(c), leader(l) {}
};

class City {
public:
    string name;
    vector<Party> parties;

    City(string n) : name(n) {
        parties.push_back(Party("PTI", 1, "Imran Khan"));
        parties.push_back(Party("PMLN", 2, "Nawaz Sharif"));
        parties.push_back(Party("MQM", 3, "Khalid Maqbool"));
    }
};

class Province {
public:
    string name;
    int code;
    vector<City> cities;

    Province(string n, int c, vector<string> cityNames) : name(n), code(c) {
        for (const string &cityName : cityNames) {
            cities.push_back(City(cityName));
        }
    }
};

class Election {
private:
    vector<Province> provinces;
    set<string> voterIDs;

public:
    Election() {
        provinces.push_back(Province("Punjab", 1, {"Lahore", "Faisalabad", "Rawalpindi"}));
        provinces.push_back(Province("Sindh", 2, {"Karachi", "Hyderabad", "Sukkur"}));
        provinces.push_back(Province("KPK", 3, {"Peshawar", "Mardan", "Abbottabad"}));
        provinces.push_back(Province("Balochistan", 4, {"Quetta", "Gwadar", "Turbat"}));
    }

    bool isVoterIDUsed(const string &voterID) {
        return voterIDs.find(voterID) != voterIDs.end();
    }

    void displayAvailableProvinces() {
        cout << "Available Provinces:\n";
        for (const auto &province : provinces) {
            cout << "Code: " << province.code << ", Province: " << province.name << endl;
        }
    }

    void displayAvailableParties() {
        cout << "Available Parties:\n";
        set<int> displayedCodes;
        for (auto &province : provinces) {
            for (auto &city : province.cities) {
                for (auto &party : city.parties) {
                    if (displayedCodes.find(party.code) == displayedCodes.end()) {
                        cout << "Code: " << party.code << ", Party: " << party.name << ", Leader: " << party.leader << endl;
                        displayedCodes.insert(party.code);
                    }
                }
            }
        }
    }

    void displayCities(int provinceCode) {
        for (auto &province : provinces) {
            if (province.code == provinceCode) {
                cout << "Cities in " << province.name << ":\n";
                for (size_t i = 0; i < province.cities.size(); ++i) {
                    cout << i + 1 << ". " << province.cities[i].name << endl;
                }
                return;
            }
        }
        cout << "Province not found!" << endl;
    }

    bool vote(string voterName, string voterID, int partyCode, int provinceCode, int cityIndex, bool isSenate) {
        if (isVoterIDUsed(voterID)) {
            cout << "Voter ID already used!" << endl;
            return false;
        }

        for (auto &province : provinces) {
            if (province.code == provinceCode) {
                if (cityIndex < 1 || cityIndex > province.cities.size()) {
                    cout << "Invalid city selection!" << endl;
                    return false;
                }

                City &city = province.cities[cityIndex - 1];
                for (auto &party : city.parties) {
                    if (party.code == partyCode) {
                        if (isSenate) {
                            party.votesSenate++;
                            cout << "Vote casted successfully for Senate - Party: " << party.name << endl;
                        } else {
                            party.votesParliament++;
                            cout << "Vote casted successfully for Parliament - Party: " << party.name << endl;
                        }
                        return true;
                    }
                }
                cout << "Invalid party code!" << endl;
                return false;
            }
        }
        cout << "Province not found!" << endl;
        return false;
    }

    void recordVoterID(string voterID) {
        voterIDs.insert(voterID);
    }

    void displayResults() {
        cout << "Election Results:\n";
        unordered_map<string, pair<int, int>> totalVotes;

        for (auto &province : provinces) {
            cout << "Province: " << province.name << endl;
            for (auto &city : province.cities) {
                cout << "  City: " << city.name << endl;
                for (auto &party : city.parties) {
                    cout << "    " << party.name << " - Senate: " << party.votesSenate
                         << " votes, Parliament: " << party.votesParliament << " votes\n";

                    totalVotes[party.name].first += party.votesSenate;
                    totalVotes[party.name].second += party.votesParliament;
                }
            }
        }

        cout << "\nOverall Results:\n";
        string senateWinner, parliamentWinner;
        int maxSenateVotes = 0, maxParliamentVotes = 0;

        for (auto &entry : totalVotes) {
            cout << entry.first << " - Senate: " << entry.second.first << " votes, Parliament: " << entry.second.second << " votes\n";
            if (entry.second.first > maxSenateVotes) {
                maxSenateVotes = entry.second.first;
                senateWinner = entry.first;
            }
            if (entry.second.second > maxParliamentVotes) {
                maxParliamentVotes = entry.second.second;
                parliamentWinner = entry.first;
            }
        }

        cout << "\nSenate Winner: " << senateWinner << endl;
        cout << "Parliament Winner: " << parliamentWinner << endl;
    }
};

int main() {
    Election election;
    string voterName, voterID;
    int partyCode, provinceCode, cityIndex;
    char choice;

    do {
        cout << "Enter your name: ";
        cin >> voterName;
        cout << "Enter your voter ID: ";
        cin >> voterID;

        if (election.isVoterIDUsed(voterID)) {
            cout << "Voter ID already used!" << endl;
            continue;
        }

        election.displayAvailableProvinces();
        cout << "Select the province by entering the code: ";
        cin >> provinceCode;

        election.displayCities(provinceCode);
        cout << "Select the city by entering the number: ";
        cin >> cityIndex;

        election.displayAvailableParties();
        cout << "Enter the code of the party you want to vote for Senate: ";
        cin >> partyCode;
        if (!election.vote(voterName, voterID, partyCode, provinceCode, cityIndex, true)) {
            continue;
        }

        cout << "Enter the code of the party you want to vote for Parliament: ";
        cin >> partyCode;
        if (!election.vote(voterName, voterID, partyCode, provinceCode, cityIndex, false)) {
            continue;
        }

        election.recordVoterID(voterID);

        cout << "Do you want to cast another vote? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    election.displayResults();
    return 0;
}
