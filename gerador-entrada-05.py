from random import randint
lista = []

arq = open("entradaHASHING-05.txt", 'w')

for x in range(0,1000):
	num = randint(100000, 1000000)
	while(num in lista):
		num = randint(100000, 1000000)
	lista.append(num)
	arq.write(str(num)+'\n')

arq.close()
