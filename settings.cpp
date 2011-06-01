#include <QtGui>
#include <settings.h>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {

  okButton = new QPushButton(tr("&OK"));
  connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
  okButton->setEnabled(false);
  cancelButton = new QPushButton(tr("&Cancel"));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));

  host = "";
  port = "";
  url  = "";
  autoRefresh = Qt::Unchecked;
  interval = 3;

  hostLineEdit = new QLineEdit();
  hostLineEdit->setText(host);
  connect(hostLineEdit, SIGNAL(textChanged(QString)), this, SLOT(hostChanged(QString)));
  portLineEdit = new QLineEdit();
  portLineEdit->setText(port);
  connect(portLineEdit, SIGNAL(textChanged(QString)), this, SLOT(portChanged(QString)));
  urlLineEdit  = new QLineEdit();
  urlLineEdit->setText(url);
  connect(urlLineEdit, SIGNAL(textChanged(QString)), this, SLOT(urlChanged(QString)));
  intervalSpinBox = new QSpinBox();
  intervalSpinBox->setMaximum(600);
  intervalSpinBox->setMinimum(3);
  intervalSpinBox->setValue(interval);
  intervalSpinBox->setEnabled(false);
  connect(intervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(intervalChanged(int)));
  autoRefreshCheckBox = new QCheckBox();
  autoRefreshCheckBox->setCheckState(autoRefresh); 
  connect(autoRefreshCheckBox, SIGNAL(stateChanged(int)), this, SLOT(autoRefreshChanged(int)));
  

  hostLabel = new QLabel(tr("Host name: "));
  hostLabel->setBuddy(hostLineEdit);
  portLabel = new QLabel(tr("Port: "));
  portLabel->setBuddy(portLineEdit);
  urlLabel  = new QLabel(tr("URL: "));
  urlLabel->setBuddy(urlLineEdit);
  intervalLabel = new QLabel(tr("Interval: "));
  intervalLabel->setBuddy(intervalSpinBox);
  autoRefreshLabel = new QLabel(tr("Auto Refresh: "));
  autoRefreshLabel->setBuddy(autoRefreshCheckBox);


  //Layouts
  QHBoxLayout *buttonsLayout = new QHBoxLayout();
  buttonsLayout->addWidget(okButton);
  buttonsLayout->addWidget(cancelButton);

  QHBoxLayout *hostLayout = new QHBoxLayout();
  hostLayout->addWidget(hostLabel);
  hostLayout->addWidget(hostLineEdit);

  QHBoxLayout *portLayout = new QHBoxLayout();
  portLayout->addWidget(portLabel);
  portLayout->addWidget(portLineEdit);

  QHBoxLayout *urlLayout = new QHBoxLayout();
  urlLayout->addWidget(urlLabel);
  urlLayout->addWidget(urlLineEdit);

  QHBoxLayout *autoRefreshLayout = new QHBoxLayout();
  autoRefreshLayout->addWidget(autoRefreshLabel);
  autoRefreshLayout->addWidget(autoRefreshCheckBox);

  QHBoxLayout *intervalLayout = new QHBoxLayout();
  intervalLayout->addWidget(intervalLabel);
  intervalLayout->addWidget(intervalSpinBox);

  QVBoxLayout *itemsLayout = new QVBoxLayout();
  itemsLayout->addLayout(hostLayout);
  itemsLayout->addLayout(portLayout);
  itemsLayout->addLayout(urlLayout);
  itemsLayout->addLayout(autoRefreshLayout);
  itemsLayout->addLayout(intervalLayout);
  
  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addLayout(itemsLayout);
  mainLayout->addLayout(buttonsLayout);

  setLayout(mainLayout);
};

void SettingsDialog::okButtonClicked() {
  emit applyed();
  hide();
};

void SettingsDialog::cancelButtonClicked() {
  emit canceled();
  hide();
};

void SettingsDialog::hostChanged(QString h) {
  okButton->setEnabled(true);
  host = h;
};

void SettingsDialog::portChanged(QString p) {
  okButton->setEnabled(true);
  port = p;
};

void SettingsDialog::urlChanged(QString u) {
  okButton->setEnabled(true);
  url = u;
};

void SettingsDialog::intervalChanged(int i) {
  okButton->setEnabled(true);
  interval = i;
};

void SettingsDialog::autoRefreshChanged(int a) {
  okButton->setEnabled(true);
  if(a == Qt::Unchecked) {
    intervalSpinBox->setEnabled(false);
    interval = 3;
  }
  else {
    intervalSpinBox->setEnabled(true);
    interval = intervalSpinBox->value();
  }
};

QString SettingsDialog::getHost() {
  return host;
};

QString SettingsDialog::getPort() {
  return port;
};

QString SettingsDialog::getUrl() {
  return url;
};

int SettingsDialog::getInterval() {
  return interval;
};

bool SettingsDialog::getAutoRefresh() {
  if(autoRefreshCheckBox->checkState() == Qt::Unchecked)
    return false;
  else
    return true;
};

void SettingsDialog::setHost(QString h) {
  host = h;
};

void SettingsDialog::setPort(QString p) {
  port = p;
};

void SettingsDialog::setUrl(QString u) {
  url = u;
};

void SettingsDialog::setInterval(int i) {
  interval = i;
};

void SettingsDialog::setAutoRefresh(bool a) {
  if(a)
    autoRefresh = Qt::Checked;
  else
    autoRefresh = Qt::Unchecked;
};

void SettingsDialog::showEvent(QShowEvent *event) {
  hostLineEdit->setText(host);
  portLineEdit->setText(port);
  urlLineEdit->setText(url);
  intervalSpinBox->setValue(interval);
  autoRefreshCheckBox->setCheckState(autoRefresh);
};

