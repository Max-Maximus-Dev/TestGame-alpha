#include <iostream>
#include <string>
#include <vector>
#include "Exceptions.h"
#include "LogicOfProgarm.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
int count_of_admins = 0;
std::vector<User*> users;
std::vector<categoryOfTests*> categories;
std::vector<Test*> all_tests;

// ====== Answer ======
Answer::Answer(std::string answer_text, bool is_correct) {
	this->answer_text = answer_text;
	this->is_correct = is_correct;
}
std::string Answer::getAnswerText() { return answer_text; }
bool Answer::getIsCorrect() { return is_correct; }

// ====== Question ======
Question::Question(std::string question_text) { this->question_text = question_text; }
std::string Question::getQuestionText() { return question_text; }
std::vector<Answer*> Question::getAnswers() { return answers; }
void Question::addAnswer(Answer* answer) { answers.push_back(answer); }

// ====== Test ======
Test::Test(std::string test_name) { this->test_name = test_name; }
std::string Test::getTestName() { return test_name; }
std::vector<Question*> Test::getQuestions() { return questions; }
void Test::addQuestion(Question* question) { questions.push_back(question); }

// ====== categoryOfTests ======
categoryOfTests::categoryOfTests(std::string category_name) { this->category_name = category_name; }
std::string categoryOfTests::getCategoryName() { return category_name; }
std::vector<Test*> categoryOfTests::getTests() { return tests; }
void categoryOfTests::addTest(Test* test) { tests.push_back(test); }

// ====== Administrator ======
Administrator::Administrator(std::string login, std::string password) {
	count_of_admins++;
	if (count_of_admins > 1) {
		throw theMaximumNumberOfAdminsIsExceeded();
	}
	this->login = login;
	this->password = password;
	this->role = "Administrator";
}
std::string Administrator::getRole() { return role; }
std::string Administrator::getLogin() { return login; }
std::string Administrator::getPassword() { return password; }
void Administrator::setLogin(std::string login) { this->login = login; }
void Administrator::setPassword(std::string password) { this->password = password; }

// ====== Guest ======
Guest::Guest(std::string login, std::string password) {
	this->login = login;
	this->password = password;
	this->role = "Guest";
	this->number_of_tests_passed = 0;
}
std::vector<double>& Guest::getGrades() { return grades; }
std::string Guest::getRole() { return role; }
int Guest::getNumberOfTestsPassed() { return number_of_tests_passed; }
void Guest::incrementTestsPassed() { number_of_tests_passed++; }
std::string Guest::getLogin() { return login; }
std::string Guest::getPassword() { return password; }
void Guest::setLogin(std::string login) { this->login = login; }
void Guest::setPassword(std::string password) { this->password = password; }
void Guest::addGrade(double g) { grades.push_back(g); }
void Guest::setNumberOfTestsPassed(int n) { number_of_tests_passed = n; }

void passTest(Guest* guest, Test* test) {
	std::cout << "Starting test: " << test->getTestName() << std::endl;
	double correct_answers = 0;
	double interest_of_correct_answers = 0;
	double grade = 0; 
	for (auto question : test->getQuestions()) {
		std::cout << question->getQuestionText() << std::endl;
		int answer_index = 1;
		for (auto answer : question->getAnswers()) {
			std::cout << answer_index << ". " << answer->getAnswerText() << std::endl;
			answer_index++;
		}
		int user_answer;
		std::cout << "Enter the number of your answer: ";
		std::cin >> user_answer;
		if (user_answer < 1 || user_answer > question->getAnswers().size()) {
			std::cout << "Invalid answer. Moving to the next question." << std::endl;
			continue;
		}
		if (question->getAnswers()[user_answer - 1]->getIsCorrect()) {
			correct_answers++;
		}
		int ask;
		std::cout << "Do you want to continue the test? (1 - Yes, 0 - No): ";
		std::cin >> ask;
		if (ask == 0) {
			break;
		}
		else if (ask != 1) {
			std::cout << "Invalid input. Continuing the test." << std::endl;
		}
	}
	interest_of_correct_answers = (correct_answers / test->getQuestions().size()) * 100;
	if (interest_of_correct_answers >= 91.67) {
		grade = 12;
	}
	else if (interest_of_correct_answers >= 83.33) {
		grade = 11;
	}
	else if (interest_of_correct_answers >= 75) {
		grade = 10;
	}
	else if (interest_of_correct_answers >= 66.67) {
		grade = 9;
	}
	else if (interest_of_correct_answers >= 58.33) {
		grade = 8;
	}
	else if (interest_of_correct_answers >= 50) {
		grade = 7;
	}
	else if (interest_of_correct_answers >= 41.67) {
		grade = 6;
	}
	else if (interest_of_correct_answers >= 33.33) {
		grade = 5;
	}
	else if (interest_of_correct_answers >= 25) {
		grade = 4;
	}
	else if (interest_of_correct_answers >= 16.67) {
		grade = 3;
	}
	else if (interest_of_correct_answers >= 8.33) {
		grade = 2;
	}
	else {
		grade = 1;
	}
	guest->incrementTestsPassed();
	guest->getGrades().push_back(grade);
	std::cout << "Test completed. Count of correct answers: " << correct_answers << " out of "
		<< test->getQuestions().size()
		<< ". Interest of correct answers: "
		<< interest_of_correct_answers
		<< "%. Grade: " << grade << std::endl;
}



void createGuestAccount(Administrator* admin, std::vector<User*>& users, std::string login, std::string password) {
	Guest* guest = new Guest(login, password);
	users.push_back(guest);
	std::cout << "Guest account created successfully.\n";
}
void deleteGuestAccount(Administrator* admin, std::vector<User*>& users, std::string login) {
	for (auto it = users.begin(); it != users.end(); it++) {
		if ((*it)->getRole() == "Guest") {
			Guest* guest = dynamic_cast<Guest*>(*it);
			if (guest && guest->getLogin() == login) {
				delete guest;
				users.erase(it);
				std::cout << "Guest account deleted successfully.\n";
				return;
			}
		}
	}
	std::cerr << "Guest account not found.\n";
}
void manageLoginPasswordGuest(Administrator* admin, std::vector<User*>& users, std::string login) {
	std::cout << "What do you want to change?\n1. Login\n2. Password\n";
	int choice;
	std::cin >> choice;
	for (auto user : users) {
		if (user->getRole() == "Guest") {
			Guest* guest = dynamic_cast<Guest*>(user);
			if (guest && guest->getLogin() == login) {
				if (choice == 1) {
					std::cout << "Enter new login: ";
					std::string new_login;
					std::cin >> new_login;
					guest->setLogin(new_login);
					std::cout << "Login changed successfully.\n";
				}
				else if (choice == 2) {
					std::cout << "Enter new password: ";
					std::string new_password;
					std::cin >> new_password;
					guest->setPassword(new_password);
					std::cout << "Password changed successfully.\n";
				}
				else {
					std::cerr << "Invalid choice.\n";
				}
				return;
			}
		}
	}
}

void showStatistics(Administrator* admin, std::vector<User*>& users, std::vector<categoryOfTests*>& categories, std::vector<Test*>& all_tests) {
	fs::current_path("DataBase");
	std::fstream results_of_tests_by_specific_category("results_of_tests_by_category.txt", std::ios::out);
	std::fstream results_of_tests_by_specific_test("results_of_tests_by_specific_test.txt", std::ios::out);
	std::fstream results_of_tests_by_specific_guest("results_of_tests_by_specific_guest.txt", std::ios::out);
	if (!results_of_tests_by_specific_category.is_open() || !results_of_tests_by_specific_test.is_open() || !results_of_tests_by_specific_guest.is_open()) {
		std::cerr << "Error opening statistics files.\n";
		return;
	}
	for (auto category : categories) {
		results_of_tests_by_specific_category << "Category: " << category->getCategoryName() << std::endl;
		for (auto test : category->getTests()) {
			results_of_tests_by_specific_category << "  Test: " << test->getTestName() << std::endl;
			for (auto user : users) {
				if (user->getRole() == "Guest") {
					Guest* guest = dynamic_cast<Guest*>(user);
					if (guest) {
						results_of_tests_by_specific_category << "    Guest: " << guest->getLogin() << ", Tests Passed: " << guest->getNumberOfTestsPassed() << ", Grades: ";
						for (auto grade : guest->getGrades()) {
							results_of_tests_by_specific_category << grade << " ";
						}
						results_of_tests_by_specific_category << std::endl;
					}
				}
			}
		}
		results_of_tests_by_specific_category << std::endl;
	}
	for (auto test : all_tests) {
		results_of_tests_by_specific_test << "Test: " << test->getTestName() << std::endl;
		for (auto user : users) {
			if (user->getRole() == "Guest") {
				Guest* guest = dynamic_cast<Guest*>(user);
				if (guest) {
					results_of_tests_by_specific_test << "  Guest: " << guest->getLogin() << ", Tests Passed: " << guest->getNumberOfTestsPassed() << ", Grades: ";
					for (auto grade : guest->getGrades()) {
						results_of_tests_by_specific_test << grade << " ";
					}
					results_of_tests_by_specific_test << std::endl;
				}
			}
		}
		results_of_tests_by_specific_test << std::endl;
	}
	for (auto user : users) {
		if (user->getRole() == "Guest") {
			Guest* guest = dynamic_cast<Guest*>(user);
			if (guest) {
				results_of_tests_by_specific_guest << "Guest: " << guest->getLogin() << ", Tests Passed: " << guest->getNumberOfTestsPassed() << ", Grades: ";
				for (auto grade : guest->getGrades()) {
					results_of_tests_by_specific_guest << grade << " ";
				}
				results_of_tests_by_specific_guest << std::endl;
			}
		}
	}
	results_of_tests_by_specific_category.close();
	results_of_tests_by_specific_test.close();
	results_of_tests_by_specific_guest.close();
	fs::current_path("..");
	std::cout << "Statistics generated successfully. Open folder DataBase!\n";
}
void addCategoryOfTests(Administrator* admin, std::vector<categoryOfTests*>& categories, std::string category_name) {
	categoryOfTests* category = new categoryOfTests(category_name);
	categories.push_back(category);
	std::cout << "Category of tests added successfully.\n";
}
void deleteCategoryOfTests(Administrator* admin, std::vector<categoryOfTests*>& categories, std::string category_name) {
	for (auto it = categories.begin(); it != categories.end(); it++) {
		if ((*it)->getCategoryName() == category_name) {
			delete* it;
			categories.erase(it);
			std::cout << "Category of tests deleted successfully.\n";
			return;
		}
	}
	std::cerr << "Category of tests not found.\n";
}
void addTest(Administrator* admin, categoryOfTests* category, std::string test_name) {
	Test* test = new Test(test_name);
	category->addTest(test);
	std::cout << "Test added successfully to category " << category->getCategoryName() << ".\n";
}
void deleteTest(Administrator* admin, categoryOfTests* category, std::string test_name) {
	auto tests = category->getTests();
	for (auto it = tests.begin(); it != tests.end(); it++) {
		if ((*it)->getTestName() == test_name) {
			delete* it;
			tests.erase(it);
			std::cout << "Test deleted successfully from category " << category->getCategoryName() << ".\n";
			return;
		}
	}
	std::cerr << "Test not found in category " << category->getCategoryName() << ".\n";
}
void addQuestionToTest(Administrator* admin, Test* test, std::string question_text, categoryOfTests* category, std::vector<Answer> answers) {
	for (auto c : category->getTests()) {
		if (c == test) {
			Question* question = new Question(question_text);
			for (auto& ans : answers) {
				Answer* answer = new Answer(ans.getAnswerText(), ans.getIsCorrect());
				question->addAnswer(answer);
			}
			test->addQuestion(question);
			std::cout << "Question added successfully to test " << test->getTestName() << ".\n";
			return;
		}
	}
	std::cerr << "Test not found in the specified category.\n";
}
void deleteQuestionFromTest(Administrator* admin, Test* test, std::string question_text, categoryOfTests* category) {
	for (auto c : category->getTests()) {
		if (c == test) {
			auto questions = test->getQuestions();
			for (auto it = questions.begin(); it != questions.end(); it++) {
				if ((*it)->getQuestionText() == question_text) {
					delete* it;
					questions.erase(it);
					std::cout << "Question deleted successfully from test " << test->getTestName() << ".\n";
					return;
				}
			}
			std::cerr << "Question not found in test " << test->getTestName() << ".\n";
			return;
		}
	}
	std::cerr << "Test not found in the specified category.\n";
}



void registerUser(std::string login, std::string password, std::string role) {
	if (role == "Administrator") {
		try {
			Administrator* admin = new Administrator(login, password);
			users.push_back(admin);
			std::cout << "Administrator registered successfully.\n";
		}
		catch (const theMaximumNumberOfAdminsIsExceeded& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	else if (role == "Guest") {
		Guest* guest = new Guest(login, password);
		users.push_back(guest);
		std::cout << "Guest registered successfully.\n";
	}
	else {
		std::cerr << "Invalid role specified.\n";
	}
}
User* loginUser(std::string login, std::string password) {
	for (auto user : users) {
		if (user->getRole() == "Administrator") {
			Administrator* admin = dynamic_cast<Administrator*>(user);
			if (admin && admin->getLogin() == login && admin->getPassword() == password) {
				std::cout << "Administrator logged in successfully.\n";
				return admin;
			}
		}
		else if (user->getRole() == "Guest") {
			Guest* guest = dynamic_cast<Guest*>(user);
			if (guest && guest->getLogin() == login && guest->getPassword() == password) {
				std::cout << "Guest logged in successfully.\n";
				return guest;
			}
		}
	}
	std::cerr << "Invalid login or password.\n";
	return nullptr;
}

void SerializeData(std::vector<User*>& users, std::vector<categoryOfTests*>& categories) {
	if (!fs::exists("DataBase\\data.txt")) {
		std::ofstream ofs("DataBase\data.txt");
		ofs.close();
	}
	fs::current_path("DataBase");
	std::ofstream ofs("data.txt");
	if (!ofs.is_open()) {
		throw dataSerializationError();
	}

	ofs << users.size() << "\n";
	for (auto& user : users) {
		ofs << user->getRole() << "\n";

		if (user->getRole() == "Administrator") {
			Administrator* admin = dynamic_cast<Administrator*>(user);
			ofs << admin->getLogin() << "\n";
			ofs << admin->getPassword() << "\n";
		}
		else if (user->getRole() == "Guest") {
			Guest* guest = dynamic_cast<Guest*>(user);
			ofs << guest->getLogin() << "\n";
			ofs << guest->getPassword() << "\n";
			ofs << guest->getNumberOfTestsPassed() << "\n";
			ofs << guest->getGrades().size() << "\n";
			for (auto grade : guest->getGrades()) {
				ofs << grade << " ";
			}
			ofs << "\n";
		}
	}

	ofs << categories.size() << "\n";
	for (auto& category : categories) {
		ofs << category->getCategoryName() << "\n";
		ofs << category->getTests().size() << "\n";

		for (auto test : category->getTests()) {
			ofs << test->getTestName() << "\n";
			ofs << test->getQuestions().size() << "\n";

			for (auto question : test->getQuestions()) {
				ofs << question->getQuestionText() << "\n";
				ofs << question->getAnswers().size() << "\n";

				for (auto answer : question->getAnswers()) {
					ofs << answer->getAnswerText() << "\n";
					ofs << (answer->getIsCorrect() ? 1 : 0) << "\n";
				}
			}
		}
	}
	ofs.close();
	fs::current_path("..");
}
void DeserializeData(std::vector<User*>& users, std::vector<categoryOfTests*>& categories) {
	if (!fs::exists("DataBase/data.txt")) {
		throw dataDeserializationError();
	}
	fs::current_path("DataBase");
	std::ifstream ifs("data.txt");
	if (!ifs.is_open()) {
		throw dataDeserializationError();
	}

	int users_size;
	ifs >> users_size;
	ifs.ignore(); 

	for (int i = 0; i < users_size; i++) {
		std::string role;
		std::getline(ifs, role);

		if (role == "Administrator") {
			std::string login, password;
			std::getline(ifs, login);
			std::getline(ifs, password);
			Administrator* admin = new Administrator(login, password);
			users.push_back(admin);
		}
		else if (role == "Guest") {
			std::string login, password;
			std::getline(ifs, login);
			std::getline(ifs, password);

			int number_of_tests_passed;
			ifs >> number_of_tests_passed;
			int grades_size;
			ifs >> grades_size;
			ifs.ignore();

			Guest* guest = new Guest(login, password);
			guest->setNumberOfTestsPassed(number_of_tests_passed);

			for (int j = 0; j < grades_size; j++) {
				double grade;
				ifs >> grade;
				guest->addGrade(grade);
			}
			ifs.ignore();
			users.push_back(guest);
		}
	}

	int categories_size;
	ifs >> categories_size;
	ifs.ignore();

	for (int i = 0; i < categories_size; i++) {
		std::string category_name;
		std::getline(ifs, category_name);
		categoryOfTests* category = new categoryOfTests(category_name);
		categories.push_back(category);

		int tests_size;
		ifs >> tests_size;
		ifs.ignore();

		for (int j = 0; j < tests_size; j++) {
			std::string test_name;
			std::getline(ifs, test_name);
			Test* test = new Test(test_name);
			category->addTest(test);

			int questions_size;
			ifs >> questions_size;
			ifs.ignore();

			for (int k = 0; k < questions_size; k++) {
				std::string question_text;
				std::getline(ifs, question_text);
				Question* question = new Question(question_text);
				test->addQuestion(question);

				int answers_size;
				ifs >> answers_size;
				ifs.ignore();

				for (int l = 0; l < answers_size; l++) {
					std::string answer_text;
					std::getline(ifs, answer_text);
					int is_correct_int;
					ifs >> is_correct_int;
					ifs.ignore();
					bool is_correct = (is_correct_int == 1);
					Answer* answer = new Answer(answer_text, is_correct);
					question->addAnswer(answer);
				}
			}
		}
	}
	ifs.close();
	fs::current_path("..");
}
