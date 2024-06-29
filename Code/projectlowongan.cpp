#include <iostream>
#include <string>
#include <ctime> // Untuk std::tm
#include <iomanip> // Untuk std::put_time
#include <regex>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

// Struktur untuk menyimpan informasi tentang lowongan pekerjaan
struct Job {
    string title;
    string description;
    string requirements;
    int maxApplicants; // Jumlah pelamar maksimum yang dapat diterima
    int acceptedApplicantsCount; // Jumlah pelamar yang telah diterima
    Job* next;
};

// Struktur untuk menyimpan informasi tentang pelamar
struct Applicant {
    string name;
    string email;
    string resume;
    std::tm registrationDate; // Tanggal pendaftaran
    std::tm interviewDate; // Tanggal wawancara
    bool accepted; // Menunjukkan apakah pelamar diterima atau tidak
    Applicant* next;
};

class JobManager {
private:
    Job* headJob;
    Applicant* headApplicant;

    bool isValidEmail(const string& email) {
        // Format email: harus berakhiran dengan "@gmail.com"
        regex pattern(R"([a-zA-Z0-9._%+-]+@gmail\.com)");
        return regex_match(email, pattern);
    }

public:
    // Konstruktor
    JobManager() {
        headJob = nullptr;
        headApplicant = nullptr;
    }

    // Fungsi untuk menghitung jumlah pelamar dan persentase yang diterima
    void calculateAcceptanceRate() {
        int totalApplicants = 0;
        int acceptedApplicants = 0;

        // Menghitung total pelamar dan pelamar yang diterima
        Applicant* current = headApplicant;
        while (current != nullptr) {
            totalApplicants++;
            if (current->accepted) {
                acceptedApplicants++;
            }
            current = current->next;
        }

        // Menghitung persentase pelamar yang diterima
        float acceptanceRate = (acceptedApplicants / static_cast<float>(totalApplicants)) * 100;

        // Menampilkan hasil
        cout << "Total Applicants: " << totalApplicants << endl;
        cout << "Accepted Applicants: " << acceptedApplicants << endl;
        cout << "Acceptance Rate: " << acceptanceRate << "%" << endl;
    }

    // Fungsi untuk menambahkan lowongan pekerjaan baru
    void addJob(string title, string description, string requirements, int maxApplicants) {
        Job* newJob = new Job;
        newJob->title = title;
        newJob->description = description;
        newJob->requirements = requirements;
        newJob->maxApplicants = maxApplicants;
        newJob->acceptedApplicantsCount = 0;
        newJob->next = headJob;
        headJob = newJob;
    }

    // Fungsi untuk menambahkan pelamar baru
    void addApplicant(string name, string email, string resume, std::tm registrationDate) {
        if (!isValidEmail(email)) {
            cout << "Invalid email format. Please use a Gmail account." << endl;
            return;
        }

        Applicant* newApplicant = new Applicant;
        newApplicant->name = name;
        newApplicant->email = email;
        newApplicant->resume = resume;
        newApplicant->registrationDate = registrationDate;
        newApplicant->next = nullptr;
        newApplicant->accepted = false;

        // Menemukan pekerjaan yang cocok dengan persyaratan pelamar
        Job* currentJob = headJob;
        bool jobFound = false;
        while (currentJob != nullptr) {
            if (resume == currentJob->requirements) {
                if (currentJob->acceptedApplicantsCount < currentJob->maxApplicants) {
                    jobFound = true;
                    newApplicant->accepted = true;
                    currentJob->acceptedApplicantsCount++;
                    break;
                }
            }
            currentJob = currentJob->next;
        }

        if (!jobFound) {
            cout << "No matching job found for the applicant or job is full. Application rejected." << endl;
        } else {
            cout << "Lamaran diterima, Silahkan mengecek jadwal wawancara!" << endl;
        }

        // Menambahkan pelamar ke daftar pelamar
        newApplicant->next = headApplicant;
        headApplicant = newApplicant;
    }

    // Fungsi untuk menentukan tanggal wawancara
    void scheduleInterview(string applicantName) {
        Applicant* current = headApplicant;
        while (current != nullptr) {
            if (current->name == applicantName) {
                // Menghitung tanggal wawancara (misalnya 5 hari setelah tanggal pendaftaran)
                std::tm interviewDate = current->registrationDate;
                interviewDate.tm_mday += 5; // Menambah 5 hari

                // Menangani perubahan bulan jika jumlah hari melebihi hari dalam bulan
                while (interviewDate.tm_mday > 30) {
                    std::mktime(&interviewDate); // Menormalkan tanggal
                    interviewDate.tm_mday -= 30; // Mengurangi hari hingga tanggal valid dalam bulan yang sama
                    ++interviewDate.tm_mon; // Pindah ke bulan berikutnya
                    if (interviewDate.tm_mon != current->registrationDate.tm_mon) {
                        // Adjust year if the month has changed
                        if (interviewDate.tm_mon == 12) {
                            // If the month becomes January, increment the year
                            interviewDate.tm_year += 1;
                        }
                    }
                }
                // Handle case where the month changes
                std::mktime(&interviewDate);

                cout << "Interview scheduled for " << put_time(&interviewDate, "%Y-%m-%d") << endl;
                cout << "Interview scheduled successfully for " << current->name << ". Please inform the applicant." << endl;
                current->interviewDate = interviewDate; // Assign the calculated interview date
                return;
            }
            current = current->next;
        }
        cout << "Applicant not found." << endl;
    }

    // Fungsi untuk mendapatkan semua pelamar
    void getAllApplicants() {
        Applicant* current = headApplicant;
        while (current != nullptr) {
            // Menampilkan nama, email, resume, tanggal pendaftaran, dan tanggal wawancara
            cout << "Name: " << current->name << endl;
            cout << "Email: " << current->email << endl;
            cout << "Resume: " << current->resume << endl;
            cout << "Registration Date: " << put_time(&current->registrationDate, "%Y-%m-%d") << endl;
            cout << "Interview Date: " << put_time(&current->interviewDate, "%Y-%m-%d") << endl;
            cout << "Accepted: " << (current->accepted ? "Yes" : "No") << endl;
            cout << endl;
            current = current->next;
        }
    }

    void evaluateApplicants() {
        cout << "Evaluating Applicants:" << endl;
        Applicant* current = headApplicant;
        while (current != nullptr) {
            // Misalkan ada kesesuaian jika kata kunci tertentu dari persyaratan pekerjaan ada dalam resume
            bool matchFound = false;
            Job* currentJob = headJob;
            while (currentJob != nullptr) {
                if (current->resume == currentJob->requirements) {
                    matchFound = true;
                    break;
                }
                currentJob = currentJob->next;
            }

            // Print other information of the applicant
            cout << "Name: " << current->name << endl;
            cout << "Email: " << current->email << endl;
            cout << "Resume: " << current->resume << endl;
            cout << "Registration Date: " << put_time(&current->registrationDate, "%Y-%m-%d") << endl;
            cout << "Interview Date: " << put_time(&current->interviewDate, "%Y-%m-%d") << endl;
            // Print status
            cout << "Status: ";
            if (matchFound && current->accepted) {
                cout << "Accepted" << endl;
                // Menampilkan pekerjaan yang diterima oleh pelamar
                cout << "Job Accepted: " << currentJob->title << endl;
            } else {
                cout << "Not Accepted" << endl;
                // Set status pelamar sebagai "Not Accepted" jika tidak ada pekerjaan yang cocok
                current->accepted = false;
            }

            cout << endl;

            current = current->next;
        }
    }

    // Fungsi untuk menghapus lowongan pekerjaan berdasarkan judul
    void removeJob(string title) {
        Job* current = headJob;
        Job* prev = nullptr;

        while (current != nullptr) {
            if (current->title == title) {
                if (prev == nullptr) {
                    headJob = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    // Fungsi untuk mendapatkan informasi tentang lowongan pekerjaan berdasarkan judul
    Job* getJobInfo(string title) {
        Job* current = headJob;
        while (current != nullptr) {
            if (current->title == title) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    // Fungsi untuk menampilkan informasi job
    void displayJobInfo(Job* job) {
        if (job == nullptr) {
            cout << "Job not found." << endl;
            return;
        }
        cout << "Job Title: " << job->title << endl;
        cout << "Description: " << job->description << endl;
        cout << "Requirements: " << job->requirements << endl;
        cout << "Max Applicants: " << job->maxApplicants << endl;
        cout << "Accepted Applicants: " << job->acceptedApplicantsCount << endl;
    }

    // Fungsi untuk mendapatkan semua lowongan pekerjaan
    void getAllJobs() {
        Job* current = headJob;
        while (current != nullptr) {
            cout << "Title: " << current->title << endl;
            cout << "Description: " << current->description << endl;
            cout << "Requirements: " << current->requirements << endl;
            cout << "Max Applicants: " << current->maxApplicants << endl;
            cout << "Accepted Applicants: " << current->acceptedApplicantsCount << endl;
            cout << endl;
            current = current->next;
        }
    }

    void matchApplicantWithJob(string applicantRequirements) {
        Applicant* currentApplicant = headApplicant;
        while (currentApplicant != nullptr) {
            Job* currentJob = headJob;
            while (currentJob != nullptr) {
                // Memeriksa apakah persyaratan pelamar cocok dengan persyaratan pekerjaan
                if (applicantRequirements == currentJob->requirements) {
                    // Menjadwalkan wawancara untuk pelamar yang cocok dengan pekerjaan
                    currentApplicant->interviewDate = currentApplicant->registrationDate;
                    currentApplicant->interviewDate.tm_mday += 5; // Misalnya, 5 hari setelah pendaftaran
                    std::mktime(&currentApplicant->interviewDate);
                    cout << "Interview scheduled for " << put_time(&currentApplicant->interviewDate, "%Y-%m-%d") << endl;
                    break;
                }
                currentJob = currentJob->next;
            }
            currentApplicant = currentApplicant->next;
        }
    }

    // Fungsi untuk menampilkan antrean wawancara yang telah terdaftar, diurutkan berdasarkan waktu wawancara
    void viewInterviewQueue() {
        vector<Applicant*> interviewQueue;

        // Mengumpulkan semua pelamar yang telah dijadwalkan wawancara
        Applicant* current = headApplicant;
        while (current != nullptr) {
            if (current->accepted) {
                interviewQueue.push_back(current);
            }
            current = current->next;
        }

        // Mengurutkan pelamar berdasarkan tanggal wawancara
        sort(interviewQueue.begin(), interviewQueue.end(), [](Applicant* a, Applicant* b) {
            return mktime(&a->interviewDate) < mktime(&b->interviewDate);
        });

        // Menampilkan antrean wawancara
        cout << "Interview Queue:" << endl;
        for (Applicant* applicant : interviewQueue) {
            cout << "Name: " << applicant->name << endl;
            cout << "Interview Date: " << put_time(&applicant->interviewDate, "%Y-%m-%d") << endl;
            cout << endl;
        }
    }
};

int main() {
    JobManager jobManager;
    int choice;
    string title, description, requirements;
    int maxApplicants; // Variabel untuk menyimpan jumlah pelamar yang dapat diterima
    string name, email, resume;
    std::tm registrationDate; // Variabel untuk tanggal pendaftaran

    do {
        // Menampilkan menu
        cout << "Menu:" << endl;
        cout << "1. Add Job" << endl;
        cout << "2. Get All Jobs" << endl;
        cout << "3. Remove Job" << endl;
        cout << "4. Search Job" << endl;
        cout << "5. Add Applicant" << endl;
        cout << "6. Schedule Interview" << endl;
        cout << "7. Evaluate Applicants" << endl;
        cout << "8. View Interview Queue" << endl;
        cout << "9. Analyze Effectiveness" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(); // Untuk membersihkan newline dari buffer
        Job* job = nullptr;
        switch (choice) {
            case 1:
                // Inputan untuk menambahkan lowongan pekerjaan
                cout << "Enter job title: ";
                getline(cin, title);
                cout << "Enter job description: ";
                getline(cin, description);
                cout << "Enter job requirements: ";
                getline(cin, requirements);
                cout << "Enter maximum number of applicants: ";
                cin >> maxApplicants;
                cin.ignore(); // Untuk membersihkan newline dari buffer
                // Menambahkan lowongan pekerjaan berdasarkan inputan
                jobManager.addJob(title, description, requirements, maxApplicants);
                break;
            case 2:
                // Menampilkan semua lowongan pekerjaan
                cout << "All Jobs:" << endl;
                jobManager.getAllJobs();
                break;
            case 3: 
                //inputan untuk menghapus lowongan pekerjaan
                cout << "Enter job title to remove: ";
                getline(cin, title);
                jobManager.removeJob(title);
                break;

             case 4: {
                cout << "Search job title: ";
                getline(cin, title);
                Job* job = jobManager.getJobInfo(title);
                jobManager.displayJobInfo(job);
                break;
            }
            case 5:
                // Inputan untuk menambahkan pelamar
                cout << "Enter applicant name: ";
                getline(cin, name);
                cout << "Enter applicant email: ";
                getline(cin, email);
                cout << "Enter applicant resume: ";
                getline(cin, resume);
                cout << "Enter registration date (YYYY-MM-DD): ";
                cin >> std::get_time(&registrationDate, "%Y-%m-%d"); // Membaca tanggal pendaftaran dari input
                cin.ignore(); // Untuk membersihkan newline dari buffer
                // Menambahkan pelamar berdasarkan inputan
                jobManager.addApplicant(name, email, resume, registrationDate);
                break;
            case 6:
                // Menjadwalkan tanggal wawancara
                {
                    string applicantName;
                    cout << "Enter applicant name: ";
                    getline(cin, applicantName);
                    jobManager.scheduleInterview(applicantName);
                }
                break;
            case 7:
                // Melakukan seleksi dan penilaian pelamar
                jobManager.evaluateApplicants();
                break;
            case 8:
                // Menampilkan antrean wawancara yang telah terdaftar
                jobManager.viewInterviewQueue();
                break;
            case 9:
                // Menghitung persentase penerimaan
                jobManager.calculateAcceptanceRate();
                break;
            case 0:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}
Code Proyek Struktur Data.txt
Menampilkan Code Proyek Struktur Data.txt.