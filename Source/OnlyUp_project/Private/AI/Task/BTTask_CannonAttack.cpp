#include "AI/Task/BTTask_CannonAttack.h"

#include "Actor/Obstacle/Pawn/Static/Cannon/ControlledCannon.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CannonAttack::UBTTask_CannonAttack()
{
	TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActor), UObject::StaticClass());

	IsAttackRequested.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, IsAttackRequested));
}

EBTNodeResult::Type UBTTask_CannonAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get BlackboardComponent
	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();

	// ���� ��û�� ���ٸ� ���� ��� ��ȯ
	bool Attackrequested = blackboardComponent->GetValueAsBool(IsAttackRequested.SelectedKeyName);
	if(!Attackrequested) return EBTNodeResult::Failed;

	// Get Target(Player)
	UObject* targetObject = blackboardComponent->GetValueAsObject(TargetActor.SelectedKeyName);
	if (!IsValid(targetObject)) return EBTNodeResult::Failed;

	AActor* target = Cast<AActor>(targetObject);

	AController* ownerController = Cast<AController>(OwnerComp.GetOwner());
	if (!IsValid(ownerController))  return EBTNodeResult::Failed;

	// ��Ʈ�ѷ��� �����ϴ� ��(Cannon)
	AControlledCannon* controlledPawn = Cast<AControlledCannon>(ownerController->GetPawn());

	FVector direction = (target->GetActorLocation() - controlledPawn->GetActorLocation());


	UStaticMeshComponent* cannonbomb = controlledPawn->GetCannonBomb();


	cannonbomb->AddImpulse(direction, NAME_None, true);




	return EBTNodeResult::Type::Succeeded;
}