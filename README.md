# ReadersWritersProblem

Motivação: Concorrência entre threads que desejam acessar um arquivo comum a todas, umas para leitura e outras, escrita.

Problema: Implementar o problema utilizando semáforos.

Restrições:
1- Vários leitores podem ler o arquivo ao mesmo tempo.
2- Apenas um escritor pode acessar o arquivo por vez.
3- Se o arquivo estiver sendo lido/escrito, não poderá ser escrito/lido.
4- Não há prioridade entre os tipos de threads.
