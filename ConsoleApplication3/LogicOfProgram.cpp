#include <iostream>
#include <string>
#include <vector>
#include "Exceptions.h"
#include "LogicOfProgarm.h"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
class categoryOfTests {
protected:
	std::string category_name;
	std::vector<Test*> tests;
public:
	categoryOfTests(std::string category_name) {
		this->category_name = category_name;
	}
	std::string getCategoryName() {
		return category_name;
	}
	void addTest(Test* test) {
		tests.push_back(test);
	}
	std::vector<Test*> getTests() {
		return tests;
	}
};
class Test {
protected:
	std::string test_name;
	std::vector<Question*> questions;
public:
	Test(std::string test_name) {
		this->test_name = test_name;
	}
	std::string getTestName() {
		return test_name;
	}
	void addQuestion(Question* question) {
		questions.push_back(question);
	}
	std::vector<Question*> getQuestions() {
		return questions;
	}
};
class Question {
protected:
	std::string question_text;
	std::vector<Answer*> answers;
public:
	Question(std::string question_text) {
		this->question_text = question_text;
	}
	std::string getQuestionText() {
		return question_text;
	}
	void addAnswer(Answer* answer) {
		answers.push_back(answer);
	}
	std::vector<Answer*> getAnswers() {
		return answers;
	}
};
class Answer {
protected:
	std::string answer_text;
	bool is_correct;
public:
	Answer(std::string answer_text, bool is_correct) {
		this->answer_text = answer_text;
		this->is_correct = is_correct;
	}
	std::string getAnswerText() {
		return answer_text;
	}
	bool getIsCorrect() {
		return is_correct;
	}
};





class User {
protected:
	std::string login, password, role;
	int count_of_admins = 0; // if >1 - exception
public:
	virtual std::string getRole() = 0;
	virtual ~User() {}
};
class Administrator : public User {
public:
	Administrator(std::string login, std::string password) {
		this->login = login;
		this->password = password;
		this->role = "Administrator";
		count_of_admins++;
		if (count_of_admins > 1) {
			throw theMaximumNumberOfAdminsIsExceeded();
			count_of_admins = 1;
		}
	}
	std::string getRole() override {
		return role;
	}
	std::string getLogin() {
		return login;
	}
	std::string getPassword() {
		return password;
	}
	void setLogin(std::string login) {
		this->login = login;
	}
	void setPassword(std::string password) {
		this->password = password;
	}
	~Administrator() {
		count_of_admins--;
	}
};
class Guest : public User {
protected:
	std::vector<double> grades;
	int number_of_tests_passed;
public:
	Guest(std::string login, std::string password) {
		this->login = login;
		this->password = password;
		this->role = "Guest";
		this->number_of_tests_passed = 0;
		this->grades = {};
	}
	std::vector<double> getGrades() {
		return grades;
	}
	std::string getRole() override {
		return role;
	}
	int getNumberOfTestsPassed() {
		return number_of_tests_passed;
	}
	void incrementTestsPassed() {
		number_of_tests_passed++;
	}
	std::string getLogin() {
		return login;
	}
	std::string getPassword() {
		return password;
	}
	void setLogin(std::string login) {
		this->login = login;
	}
	void setPassword(std::string password) {
		this->password = password;
	}
};



void passtest(Guest* guest, Test* test) {
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
void manageLoginPasswordGuest(Administrator* admin, std::vector<User*>& users, std::string login, std::string new_login, std::string new_password) {
	std::cout << "What do you want to change?\n1. Login\n2. Password\n";
	int choice;
	std::cin >> choice;
	for (auto user : users) {
		if (user->getRole() == "Guest") {
			Guest* guest = dynamic_cast<Guest*>(user);
			if (guest && guest->getLogin() == login) {
				if (choice == 1) {
					guest->setLogin(new_login);
					std::cout << "Login changed successfully.\n";
				}
				else if (choice == 2) {
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

void showStatistics(Administrator* admin, std::vector<User*>& users, std::vector<categoryOfTests*>& categories, std::vector<Test>& all_tests) {
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
		results_of_tests_by_specific_test << "Test: " << test.getTestName() << std::endl;
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
	std::cout << "Statistics generated successfully.\n";
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
void addQuestionToTest(Administrator* admin, Test* test, std::string question_text) {
	Question* question = new Question(question_text);
	test->addQuestion(question);
	std::cout << "Question added successfully to test " << test->getTestName() << ".\n";
}
void deleteQuestionFromTest(Administrator* admin, Test* test, std::string question_text) {
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
}



std::vector<User*> users;
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