edit:
	(cd compresor; make)
	(cd descompresor; make)

.PHONY: clean

clean:
	(cd compresor; make clean)
	(cd descompresor; make clean)