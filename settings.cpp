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
  interval = 0;

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
  autoRefreshCheckBox->setChangeState(autoRefresh); 
  connect(autoRefreshCheckBox, SIGNAL(stateChanged(int)), this, SLOT(autoRefreshChanged(int)));
  

  hostLabel = new QLabel(tr("Host name: "));
  hostLabel->setBuddy(hostLineEdit);
  portLabel = new QLabel(tr("Port: "));
  portLabel->setBuddy(portLineEdit);
  urlLabel  = new QLabel(tr("URL: "));
  urlLabel->setBuddy(urlLineEdit);
  intervalLabel = new QLabel(tr("Interval: "));
  intervalLabel->setBuddy(intervalSpinBox);
  autoRefreshLabel = new QLable(tr("Auto Refresh: "));
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
  emit applyed(host, port, url);
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
////!!!!!!!!!!!!!!!

};
