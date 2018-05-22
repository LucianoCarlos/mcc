import os
import glob
import subprocess

R1 = 1
R2 = 2


class Caixa(object):

    p = l = a = v = id = r = None

    def __init__(self, p, l, a, v, id, r=R1):
        self.p = p
        self.l = l
        self.a = a
        self.v = v
        self.id = id
        self.r = r

    def rotacao2(self):
        ''' Retorna a rotação r2 '''

        return Caixa(self.p, self.a, self.l, self.v, self.id, R2)

    @staticmethod
    def print_c(cxs):
        print('id\tlargura\taltura\tprof\tvalor\trotacao')
        for c in cxs:
            print('{0}\t{1}\t{2}\t{3}\t{4}\t{5}'.format(
                c.id, c.l, c.a, c.p, c.v, c.r)
            )


def obtem_caixas(path):
    arq = open(path).readlines()

    n = int(arq[0])
    h = int(arq[1])

    # Obtém valores de cada caixa, presente no arquivo
    valores = [int(x) for x in arq[2:n + 2]]

    # Lê caixas do arquivo criando um vetor type Caixa
    caixas = []
    id = 0
    for v, i in zip(valores, range(n+2, len(arq), 3)):

        p = int(arq[i+2])
        l = int(arq[i])
        a = int(arq[i+1])

        cx = Caixa(p, l, a, v, id+1)

        # rotaçao r1
        caixas.append(cx)

        id += 1
        # rotação r2
        caixas.append(cx.rotacao2())

    return h, caixas


def search_caixa(caixas, id, rotacao):
    return [c for c in caixas if c.id == id and c.r == rotacao][0]


def obtem_resposta_haroldo(file):
    with open(file) as arq:
        return int(arq.readline())


def verifica_resposta(path_sol, file_data, caixas, h):

    with open(file_sol) as arq:
        valor_total = int(arq.readline())
        n = int(arq.readline())

        sol_caixas = arq.readlines()

        # Verifica se tem mais caixas que o calculado no arquivo
        assert len(sol_caixas) == n

        # File possívelmente correto
        file_correto = file.split('.')[0] + '.sol'
        file_correto = os.path.abspath(file_correto)

        if(os.path.isfile(file_correto)):

            res_correto = obtem_resposta_haroldo(file_correto)

            # Verifica com resultado correto
            assert res_correto == valor_total

        soma_valor = 0
        soma_altura = 0

        for i in sol_caixas:
            resposta = i.split()
            id = int(resposta[0])
            rotacao = int(resposta[1])

            # Caixa utilizada na solução
            caixa = search_caixa(caixas, id, rotacao)

            soma_valor += caixa.v
            soma_altura += caixa.a

        assert soma_valor == valor_total
        assert soma_altura <= h


# abre aquivos de teste
files_data = [x for x in glob.glob('*.data')]
files_solucao_tmp = []

try:
    for file in files_data:
        print(os.path.abspath(file), end='')
        res = obtem_caixas(os.path.abspath(file))

        caixas = res[1]
        h = res[0]

        file_sol = os.path.abspath(file.split('.')[0] + '.solucao')
        files_solucao_tmp.append(file_sol)

        p = subprocess.Popen(
            ['./main', file, file_sol], stdout=subprocess.PIPE
        )
        p.wait()
        verifica_resposta(file_sol, file, caixas, h)

        print('\tOk')
except AssertionError:
    print('\n')
    raise
finally:
    # Deletando arquivos gerados
    for file in files_solucao_tmp:
        os.remove(file)
