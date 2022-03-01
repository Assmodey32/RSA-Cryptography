#include "FrontendRSA.h"
#include "../RSA/base64.h"
#include <QMessageBox>
#include <QRegExp>

FrontendRSA::FrontendRSA(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void FrontendRSA::on_pushButtonEncrypt_clicked()
{
	QString publicKey = ui.plainTextEditPublicKey->toPlainText();
	QRegExp rx("n: \\d{150,}\ne: \\d{1,9}");

	if (!rx.exactMatch(publicKey)) {
		QMessageBox::warning(this, "Invalid data", "Invalid public key!");
		return;
	}

	std::stringstream ss(publicKey.toStdString());
	std::string temp;
	ss >> temp >> temp;
	cpp_int n(temp);
	ss >> temp >> temp;
	uint32_t e(std::stoi(temp));

	rsa.setN(n);
	rsa.setE(e);

	std::string message = ui.plainTextEditTextToEncrypt->toPlainText().toStdString();

	for (auto x : message) {
		if (!std::isprint(x) && !std::isspace(x))
		{
			QMessageBox::warning(this, "Invalid data", "Invalid source text input!");
			return;
		}
	}

	if (message.empty()) {
		QMessageBox::warning(this, "Empty input", "Input field is empty!");
		return;
	}

	std::string encryptedMsg = rsa.encrypt(message);
	std::string base64Encrypted = base64_encode(encryptedMsg);

	ui.plainTextEditEncryptedText->setPlainText(base64Encrypted.c_str());
}

void FrontendRSA::on_pushButtonGenerateKeys_clicked()
{
	//QMessageBox::information(this, "Keys", "Generating...");

	popMessageBox();

	rsa.generateKeys();

	std::string n = boost::lexical_cast<std::string>(rsa.getN());

	std::string d = boost::lexical_cast<std::string>(rsa.getD());
	std::string p = boost::lexical_cast<std::string>(rsa.P());
	std::string q = boost::lexical_cast<std::string>(rsa.Q());

	ui.plainTextEditPublicKey->setPlainText(QString("n: ") + n.c_str() +
		"\n" + "e: " + std::to_string(rsa.getE()).c_str());
	ui.plainTextEditPrivateKey->setPlainText(QString("p: ") + p.c_str() +
		"\nq: "  + p.c_str() + "\nd: " + d.c_str());

}

void FrontendRSA::on_pushButtonDecrypt_clicked()
{
	QString privateKey = ui.plainTextEditPrivateKey->toPlainText();
	QRegExp rx("p: \\d{150,}\nq: \\d{150,}\nd: \\d{150,}");

	if (!rx.exactMatch(privateKey)) {
		QMessageBox::warning(this, "Invalid data", "Invalid private key!");
		return;
	}

	std::stringstream ss(privateKey.toStdString());
	std::string temp;
	ss >> temp >> temp;
	cpp_int p(temp);
	ss >> temp >> temp;
	cpp_int q(temp);
	ss >> temp >> temp;
	cpp_int d(temp);

	rsa.P(p);
	rsa.Q(q);
	rsa.setD(d);

	std::string base64Msg = ui.plainTextEditTextToDecrypt->toPlainText().toStdString();

	for (auto x : base64Msg) {
		if (!std::isprint(x) && !std::isspace(x))
		{
			QMessageBox::warning(this, "Invalid data", "Invalid encrypted text input!");
			return;
		}
	}

	if (base64Msg.empty()) {
		QMessageBox::warning(this, "Empty input", "Input field is empty!");
	}

	std::string encryptedMsg = base64_decode(base64Msg);

	ui.plainTextEditDecryptedText->setPlainText(rsa.decrypt(encryptedMsg).c_str());
}

void FrontendRSA::popMessageBox()
{
	QMessageBox* msgBox = new QMessageBox(this);
	//msgBox->setIcon(QMessageBox::Warning);
	msgBox->setWindowTitle("Generating...");
	msgBox->addButton(QMessageBox::Ok);
	msgBox->setAttribute(Qt::WA_DeleteOnClose); // delete pointer after close
	msgBox->setModal(false);
	msgBox->show();
	//msgBox->exec();
}

