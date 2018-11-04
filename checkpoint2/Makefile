all:
	@rm -rf 15-441-project-1
	@tar -mxf handin.tar
	@mkdir -p apachebench
	@cp ./ab ./apachebench
	@perl -pi -e 's/HTTP\/1.0/HTTP\/1.1/g' ./apachebench/ab
	mkdir -p tmp
	cp -r www tmp
	@cp *.py 15-441-project-1
	@cp -r unittest2 15-441-project-1
	@cp -r http_parser 15-441-project-1
	@cp -r requests 15-441-project-1
	@cp ./yacc 15-441-project-1
	sed -i 's/yacc/.\/yacc/g' 15-441-project-1/*akefile	
	cd 15-441-project-1; python ./grader1cp2.py
