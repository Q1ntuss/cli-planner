#include <iostream>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <cstdio>

using namespace std;
//structures
struct ProjectTask{
char TaskName[50];
char description[100];
bool IsComplete;
};

struct Project{
int id;
char ProjectName[50];
bool IsComplete;
char Deadline[15];
ProjectTask Tasks[20];
int TaskCount;
};
//global variables
    Project projects[50];
    int ProjectCount = 0;
    int nextProjectId = 1;
//functions

void GetToday(char today[]) {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    sprintf(today, "%04d-%02d-%02d",
            1900 + ltm->tm_year,
            1 + ltm->tm_mon,
            ltm->tm_mday);
}

int FindProjectById(int id) {
    for(int i = 0; i < ProjectCount; i++) {
        if(projects[i].id == id) {
            return i;
        }
    }
    return -1;
}

bool IsProjectOverdue(int index) {
    char today[15];
    GetToday(today);
    return (!projects[index].IsComplete && strcmp(projects[index].Deadline, today) < 0);
}

void AddProject(){
Project p;
p.id = nextProjectId++;
cin.ignore();
cout << "enter project name: ";
cin.getline(p.ProjectName, 50);
cout << endl << "enter deadline: ";
cin .getline(p.Deadline, 15);
p.IsComplete=false;
p.TaskCount=0;

projects[ProjectCount]=p;
ProjectCount++;

cout<< "Project added! ID: " << p.id << endl;
}

void AddTask(){
if(ProjectCount == 0) {
    cout << "No projects! Add a project first." << endl;
    return;
}

int projectId;
cout << "Enter project ID: ";
cin >> projectId;

int index = FindProjectById(projectId);
if(index == -1) {
    cout << "Project not found!" << endl;
    return;
}

if(projects[index].TaskCount >= 20) {
    cout << "Error: Maximum 20 tasks per project!" << endl;
    return;
}

cin.ignore();
ProjectTask t;
cout << "Task name: ";
cin.getline(t.TaskName, 50);
cout << "Task description: ";
cin.getline(t.description, 100);
t.IsComplete = false;

projects[index].Tasks[projects[index].TaskCount]=t;
projects[index].TaskCount++;
cout << "Task added!" << endl;
}

void DisplayAll(){
if(ProjectCount == 0) {
    cout << "No projects." << endl;
    return;
}
else{
    cout << "\n=== PROJECTS ===" << endl;
    for (int i=0;i<ProjectCount;i++){
//cout project info
    cout << i+1 << ". [ID: " << projects[i].id << "] " << projects[i].ProjectName
     << " | Deadline: " << projects[i].Deadline
     << " | Status: ";
    if(IsProjectOverdue(i)) {
        cout << "OVERDUE!";
    } else if(projects[i].IsComplete) {
        cout << "Complete!";
    } else {
        cout << "In Progress";
    }
    cout << endl;
//cout tasks
if(projects[i].TaskCount == 0) {
    cout << "  No tasks." << endl;
}
else{
    cout << "  Tasks:" <<endl;
        for (int j=0;j<projects[i].TaskCount;j++){
            cout << "    " << j+1 << ". ";
            if(projects[i].Tasks[j].IsComplete == true){
                cout << "[X]";
            }
            else{
                cout << "[ ]";
            }
        cout << projects[i].Tasks[j].TaskName;
        cout << " - " << projects[i].Tasks[j].description << endl;
        }
    }
}
}
}

void ViewProjectTasks(){
if(ProjectCount == 0) {
    cout << "No projects." << endl;
    return;
}

int projectId;
cout << "Enter project ID: ";
cin >> projectId;

int index = FindProjectById(projectId);
if(index == -1) {
    cout << "Project not found!" << endl;
    return;
}

cout << "\nTasks for " << projects[index].ProjectName << ":" << endl;
if(projects[index].TaskCount == 0) {
    cout << "  No tasks." << endl;
    return;
}

for(int j = 0; j < projects[index].TaskCount; j++) {
    cout << "  " << j+1 << ". ";
    if(projects[index].Tasks[j].IsComplete) {
        cout << "[X] ";
    } else {
        cout << "[ ] ";
    }
    cout << projects[index].Tasks[j].TaskName;
    cout << " - " << projects[index].Tasks[j].description << endl;
}
}

void DeleteProject(){
if(ProjectCount == 0) {
    cout << "No projects to delete." << endl;
    return;
}

int projectId;
cout << "Enter project ID to delete: ";
cin >> projectId;

int index = FindProjectById(projectId);
if(index == -1) {
    cout << "Project not found!" << endl;
    return;
}

cout << "Are you sure you want to delete project '" << projects[index].ProjectName << "'? (y/n): ";
char confirm;
cin >> confirm;
if(confirm != 'y' && confirm != 'Y') {
    cout << "Deletion cancelled." << endl;
    return;
}

for(int i = index; i < ProjectCount - 1; i++) {
    projects[i] = projects[i + 1];
}
ProjectCount--;
cout << "Project deleted!" << endl;
}

void DeleteTask(){
if(ProjectCount == 0) {
    cout << "No projects." << endl;
    return;
}

int projectId;
cout << "Enter project ID: ";
cin >> projectId;

int index = FindProjectById(projectId);
if(index == -1) {
    cout << "Project not found!" << endl;
    return;
}

if(projects[index].TaskCount == 0) {
    cout << "No tasks in this project!" << endl;
    return;
}

cout << "Tasks in " << projects[index].ProjectName << ":" << endl;
for(int j = 0; j < projects[index].TaskCount; j++) {
    cout << "  " << j+1 << ". " << projects[index].Tasks[j].TaskName << endl;
}

int taskNum;
cout << "Enter task number to delete: ";
cin >> taskNum;
taskNum--;

if(taskNum < 0 || taskNum >= projects[index].TaskCount) {
    cout << "Invalid task number!" << endl;
    return;
}

cout << "Delete task '" << projects[index].Tasks[taskNum].TaskName << "'? (y/n): ";
char confirm;
cin >> confirm;
if(confirm != 'y' && confirm != 'Y') {
    cout << "Deletion cancelled." << endl;
    return;
}


for(int i = taskNum; i < projects[index].TaskCount - 1; i++) {
    projects[index].Tasks[i] = projects[index].Tasks[i + 1];
}
projects[index].TaskCount--;
cout << "Task deleted!" << endl;
}

void AutoLoad() {
    ifstream fin("default.txt");
    if(!fin) {
        return;
    }

    fin >> ProjectCount;
    fin.ignore();

    for(int i = 0; i < ProjectCount; i++) {
        fin >> projects[i].id;
        fin.ignore();
        fin.getline(projects[i].ProjectName, 50);
        fin.getline(projects[i].Deadline, 15);
        fin >> projects[i].IsComplete;
        fin >> projects[i].TaskCount;
        fin.ignore();

        for(int j = 0; j < projects[i].TaskCount; j++) {
            fin.getline(projects[i].Tasks[j].TaskName, 50);
            fin.getline(projects[i].Tasks[j].description, 100);
            fin >> projects[i].Tasks[j].IsComplete;
            fin.ignore();
        }
    }

    fin.close();
    cout << "Auto-loaded " << ProjectCount << " projects from default.txt" << endl;
}

void AutoSave() {
    ofstream fout("default.txt");
    if(!fout) {
        return;
    }

    fout << ProjectCount << endl;

    for(int i = 0; i < ProjectCount; i++) {
        fout << projects[i].id << endl;
        fout << projects[i].ProjectName << endl;
        fout << projects[i].Deadline << endl;
        fout << projects[i].IsComplete << endl;
        fout << projects[i].TaskCount << endl;

        for(int j = 0; j < projects[i].TaskCount; j++) {
            fout << projects[i].Tasks[j].TaskName << endl;
            fout << projects[i].Tasks[j].description << endl;
            fout << projects[i].Tasks[j].IsComplete << endl;
        }
    }

    fout.close();
}

void SaveToFile() {
    char filename[50];
    cout << "Enter file name: ";
    cin.ignore();
    cin.getline(filename, 50);

    ofstream fout(filename);
    if(!fout) {
        cout << "Error creating file!" << endl;
        return;
    }

    fout << ProjectCount << endl;

    for(int i = 0; i < ProjectCount; i++) {
        fout << projects[i].id << endl;
        fout << projects[i].ProjectName << endl;
        fout << projects[i].Deadline << endl;
        fout << projects[i].IsComplete << endl;
        fout << projects[i].TaskCount << endl;

        for(int j = 0; j < projects[i].TaskCount; j++) {
            fout << projects[i].Tasks[j].TaskName << endl;
            fout << projects[i].Tasks[j].description << endl;
            fout << projects[i].Tasks[j].IsComplete << endl;
        }
    }

    fout.close();
    cout << "Saved!" << endl;
}

void LoadFromFile() {
    char filename[50];
    cout << "Enter filename: ";
    cin >> filename;
    cin.ignore();

    ifstream fin(filename);
    if(!fin) {
        cout << "Error opening file!" << endl;
        return;
    }

    fin >> ProjectCount;
    fin.ignore();

    for(int i = 0; i < ProjectCount; i++) {
        fin >> projects[i].id;
        fin.ignore();
        fin.getline(projects[i].ProjectName, 50);
        fin.getline(projects[i].Deadline, 15);
        fin >> projects[i].IsComplete;
        fin >> projects[i].TaskCount;
        fin.ignore();

        for(int j = 0; j < projects[i].TaskCount; j++) {
            fin.getline(projects[i].Tasks[j].TaskName, 50);
            fin.getline(projects[i].Tasks[j].description, 100);
            fin >> projects[i].Tasks[j].IsComplete;
            fin.ignore();
        }
    }

    fin.close();
    cout << "Loaded " << ProjectCount << " projects." << endl;
}

void ShowOverdue() {
    if(ProjectCount == 0) {
        cout << "No projects." << endl;
        return;
    }

    cout << "\n=== OVERDUE PROJECTS ===" << endl;
    bool found = false;

    for(int i = 0; i < ProjectCount; i++) {
        if(IsProjectOverdue(i)) {
            cout << i+1 << ". [ID: " << projects[i].id << "] " << projects[i].ProjectName
                 << " | Deadline: " << projects[i].Deadline
                 << " | OVERDUE!" << endl;
            found = true;
        }
    }

    if(!found) cout << "No overdue projects!" << endl;
}

int main() {
    int choice;

    AutoLoad();

    while(true) {
        cout << "\n=== PROJECT TRACKER ===" << endl;
        cout << "1. Add project" << endl;
        cout << "2. Add task" << endl;
        cout << "3. Display all" << endl;
        cout << "4. View project tasks" << endl;
        cout << "5. Show overdue" << endl;
        cout << "6. Delete project" << endl;
        cout << "7. Delete task" << endl;
        cout << "8. Save to file" << endl;
        cout << "9. Load from file" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch(choice) {
            case 1: AddProject(); break;
            case 2: AddTask(); break;
            case 3: DisplayAll(); break;
            case 4: ViewProjectTasks(); break;
            case 5: ShowOverdue(); break;
            case 6: DeleteProject(); break;
            case 7: DeleteTask(); break;
            case 8: SaveToFile(); break;
            case 9: LoadFromFile(); break;
            case 0:
                AutoSave();
                cout << "Bye!" << endl;
                return 0;
            default: cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}
