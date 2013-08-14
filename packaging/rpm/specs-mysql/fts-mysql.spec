Name: fts-mysql
Version: 3.1.0
Release: 4%{?dist}
Summary: File Transfer Service V3 mysql plug-in
Group: Applications/Internet
License: ASL 2.0
URL: https://svnweb.cern.ch/trac/fts3/wiki
# The source for this package was pulled from upstream's vcs.  Use the
# following commands to generate the tarball:
#  svn export https://svn.cern.ch/reps/fts3/tags/EPEL_release_mysql_v1
#  tar -czvf fts-mysql-0.0.1-60.tar.gz fts-mysql-00160
Source0: https://grid-deployment.web.cern.ch/grid-deployment/dms/fts3/tar/%{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  boost-devel%{?_isa}
BuildRequires:  glib2-devel%{?_isa}
BuildRequires:  soci-mysql-devel%{?_isa}
BuildRequires:  libuuid-devel%{?_isa}
Requires(pre):  shadow-utils
Requires:       soci-mysql%{?_isa}

%description
The File Transfer Service V3 mysql plug-in

%prep
%setup -qc

%build
mkdir build
cd build
%cmake -DMYSQLBUILD=ON -D CMAKE_BUILD_TYPE=RelWithDebInfo -D CMAKE_INSTALL_PREFIX='' ..
make %{?_smp_mflags}

%install
cd build
make install DESTDIR=%{buildroot}

%post   -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%{_libdir}/libfts_db_mysql.so.*
%doc %{_docdir}/fts3/mysql-schema.sql
%doc %{_docdir}/fts3/mysql-drop.sql
%doc %{_docdir}/fts3/mysql-truncate.sql
%doc README
%doc LICENSE

%changelog
* Mon Jul 29 2013 Michal Simon <michal.simon@cern.ch> - 3.1.0-1
  - First EPEL release
* Fri Jul 02 2013 Michail Salichos <michail.salichos@cern.ch> - 3.0.3-14
  - mysql queries optimization
